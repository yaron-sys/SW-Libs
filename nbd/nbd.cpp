/******************************************************************************/
/*                          C++  - BUSE - NBD                                 */ 
/******************************************************************************/
#include <iostream>
#include <mutex>
#include <dlfcn.h>  // dlopen

#include "handleton.hpp"
#include "nbd.hpp"
#include "factory.hpp"
#include "thread_pool.hpp"
#include "dir_monitor.hpp"

/******************************************************************************/
/*                                 ilrd                                       */  
/******************************************************************************/
namespace ilrd{
namespace nbd {

std::mutex reader;

inline Read::Read(const Args& args)
: NBDCommand(args), buf(operator new(args.m_len))
{}

inline Read::~Read()
{
    operator delete(buf); buf = nullptr;
}

inline void Read::do_it()
{
    if (nbd_args.m_dbug_mode) {
        std::cerr << "Request for read of size " << nbd_args.m_len << std::endl;
    }
    nbd_args.m_reply.error = nbd_args.m_storage_file->read(buf, nbd_args.m_len, nbd_args.m_offset);
    
    std::lock_guard<std::mutex> read_lock(reader);
    write_all(nbd_args.m_sk, (char*)&nbd_args.m_reply, sizeof(nbd_args.m_reply));
    write_all(nbd_args.m_sk, (char*)buf, nbd_args.m_len);
}
/******************************************************************************/
inline Write::Write(const Args& args)
: NBDCommand(args), buf(operator new(args.m_len))
{}

inline Write::~Write()
{
    operator delete(buf); buf = nullptr;
}

inline void Write::do_it()
{
    if (nbd_args.m_dbug_mode) {
        std::cerr << "Request for write of size " << nbd_args.m_len << std::endl;
    }
    
    nbd_args.m_reply.error = nbd_args.m_storage_file->write(buf, nbd_args.m_len, nbd_args.m_offset);
    
    std::lock_guard<std::mutex> read_lock(reader);
    write_all(nbd_args.m_sk, (char*)&nbd_args.m_reply, sizeof(nbd_args.m_reply));
}

inline void* Write::get_buf()
{
	return buf;
}
/******************************************************************************/
/*                          NBD Function                                      */  
/******************************************************************************/
void read_all(int fd, char* buf, size_t count)
{
    while (count > 0) {
        int bytes_read = read(fd, buf, count);
        assert(bytes_read > 0);
        buf += bytes_read;
        count -= bytes_read;
    }
    assert(count == 0);
}

/******************************************************************************/
void write_all(int fd, char* buf, size_t count)
{
    while (count > 0) {
        int bytes_written = write(fd, buf, count);
        assert(bytes_written > 0);
        buf += bytes_written;
        count -= bytes_written;
    }
    assert(count == 0);
}

/******************************************************************************/
/*                      BDproxy Class  Implementation                         */  
/******************************************************************************/
/******************************************************************************
                                   C'tors                   
******************************************************************************/
BDproxy::BDproxy(const char* dev_file, size_t dev_size, const char* stor_file_path, const char* monitor_path, bool dbug_mode)
:m_dev_file(dev_file),m_conected_status(true), m_dbug_mode(dbug_mode), 
m_storage_file(stor_file_path, dev_size, dbug_mode), m_monitor_path(monitor_path)
{    
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, m_fd) == -1){
        throw std::runtime_error("Faild in socketpair, check errno to details");
    }

    int nbd = open(dev_file, O_RDWR);
    if (nbd == -1) {
        std::string err;
        err = "Failed to open";
        err += dev_file;
        err += strerror(errno);
        err += "Is kernel module `nbd' is loaded and you have permissions";
        err += "to access the device?\n";
        throw std::runtime_error(err);
    }
    
    std::string err("Failed in ioctl: "); 
    if (ioctl(nbd, NBD_SET_SIZE, dev_size) == -1){
       err += strerror(errno); 
       throw std::runtime_error(err); 
    }
    
    if (ioctl(nbd, NBD_CLEAR_SOCK) == -1){
        err += strerror(errno); 
        throw std::runtime_error(err);
    }
    
    std::thread do_it(&BDproxy::run_nbd, this, nbd);
    m_do_it.swap(do_it);
    
    std::thread rw(&BDproxy::run_rw, this);
    m_rw.swap(rw);
}

/******************************************************************************
                            D'tors                   
******************************************************************************/
BDproxy::~BDproxy() 
{
     m_do_it.join(); 
     m_rw.join(); 
}

/******************************************************************************/
NBDCommand* create_read(const Args& args){
    return new Read(args);
}
NBDCommand* create_write(const Args& args){
    return new Write(args);
}
NBDCommand* create_disc(const Args& args){
    return new Disc(args);
}
NBDCommand* create_flush(const Args& args){
    return new Flush(args);
}
NBDCommand* create_trim(const Args& args){
    return new Trim(args);
}
/******************************************************************************/
struct ThreadPoolTask
{
    void operator()(std::shared_ptr<NBDCommand> cmd) const
    {
        cmd->do_it();
    }
};

/******************************************************************************/
class Observe {
public:
    explicit Observe(const std::string& path) : m_path(path + "/") {}
    
    void notify(MonitorEvent event)
    {
		std::string file_path(m_path);
		file_path += event.m_file_name;
        add_new_event(file_path);
    }
    void dead_notify()
    {
        std::cout << " event: DEAD" << std::endl;
    }
	void add_new_event(std::string file_name)
	{
		void *lib = dlopen(file_name.c_str(), RTLD_LAZY);
		if (lib == nullptr) {
			std::cerr << "failed to load " << file_name << " dyn lib" << std::endl;
		}
		
		dlclose(lib);
	}
private:
    const std::string m_path;
};

/******************************************************************************
                            Methods                  
******************************************************************************/
void BDproxy::run_rw()
{
    int tmp_fd;
    ssize_t bytes_read;
    
    // This is the packet used for communication between client and server.
    // All data are in network byte order.
    struct nbd_request request;
    
    // This is the reply packet that nbd-server sends back to the client after
    // it has completed an I/O request (or an error occurs).
    struct nbd_reply reply;
    
    int sk = m_fd[0];
    
    tmp_fd = open(m_dev_file, O_RDONLY);
    assert(tmp_fd != -1);
    close(tmp_fd);

    reply.magic = htonl(NBD_REPLY_MAGIC);
    reply.error = htonl(0);
    
    // create args struct
    Args args;
    
    // Detect num of CPU cores and create a thread pool
    unsigned int nthreads = std::thread::hardware_concurrency();
    if(nthreads == 0){
		nthreads = 1;
	}
    ThreadPool pool(nthreads);
    ThreadPoolTask pool_task;
    
	using Factory_h = Factory<NBDCommand, unsigned int, Args>;
    Factory_h* factory = Handleton<Factory_h>::get_instance();
    
    try{
        factory->add(NBD_CMD_READ , create_read );
        factory->add(NBD_CMD_WRITE, create_write);
        factory->add(NBD_CMD_DISC , create_disc );
        factory->add(NBD_CMD_FLUSH, create_flush);
        factory->add(NBD_CMD_TRIM , create_trim );
    }
    catch(const Factory_h::FactoryException& e) {
        std::cout << e.what() << std::endl;
    }
    
    // set monitor and observer
    DirMonitor monitor(m_monitor_path, IN_CLOSE_WRITE);
    Observe ob(m_monitor_path);
    CallBack<MonitorEvent, Observe>* observer;
	observer = new CallBack<MonitorEvent,Observe>(&ob, &Observe::notify, &Observe::dead_notify);
	monitor.subscribe(observer);

	// start monitoring
	monitor.run();

    while ((m_conected_status) && ((bytes_read = read(sk, &request, sizeof(request))) > 0)){
        assert(bytes_read == sizeof(request));
        memcpy(reply.handle, request.handle, sizeof(reply.handle));
        reply.error = htonl(0);

        assert(request.magic == htonl(NBD_REQUEST_MAGIC));
        
		// fill struct per request
        args.m_len = ntohl(request.len);
        args.m_offset = ntohll(request.from);
        args.m_dbug_mode = m_dbug_mode;
        args.m_reply = reply;
        args.m_storage_file = &m_storage_file;
        args.m_sk = sk;

        try{
            std::shared_ptr<NBDCommand> cmd = factory->create(ntohl(request.type), args);
            if(ntohl(request.type) == NBD_CMD_WRITE) {
				read_all(args.m_sk, (char *)cmd->get_buf(), args.m_len);
            }
            
            pool.add_task(ThreadPool::Priority::HIGH, pool_task, cmd);
        }
        catch(const Factory_h::FactoryException& e) {
            std::cout << e.what() << std::endl;
        }
        
        //std::memset(&request, 0, sizeof(request));
    }
    
    // clean up
    pool.stop(10);
    delete observer;
    
    if (bytes_read == -1) {
        std::cerr << strerror(errno) << std::endl;
    }
}

/******************************************************************************/
void BDproxy::run_nbd(int nbd)
{
    int sk = m_fd[1];
    
    if (ioctl(nbd, NBD_SET_SOCK, sk) == -1) {
        std::cerr << "NBD: ioctl(nbd, NBD_SET_SOCK, sp[sockChild]) failed.[" 
        << strerror(errno) << "]" << std::endl;
    }
#if defined NBD_SET_FLAGS && defined NBD_FLAG_SEND_TRIM
    else if (ioctl(nbd, NBD_SET_FLAGS, NBD_FLAG_SEND_TRIM) == -1) {
        std::cerr << "NBD: ioctl(nbd, NBD_SET_FLAGS, NBD_FLAG_SEND_TRIM) failed.[" 
        << strerror(errno) << "]" << std::endl;
    }
#endif
    else {
        int err = ioctl(nbd, NBD_DO_IT);
        if (err == -1) {
            if (errno != EPIPE) { // we're expecting a broken pipe when the parent closes it
                 std::cerr << "NBD: nbd device terminated with code " << errno 
                 << '(' << strerror(errno) << ')' << std::endl;
            }
        }
    }

    ioctl(nbd, NBD_CLEAR_QUE);
    ioctl(nbd, NBD_CLEAR_SOCK);
    m_conected_status = false;    
}

/******************************************************************************/
/*                       StorageFile Class  Implementation                    */  
/******************************************************************************/
/******************************************************************************
                            C'tors                   
******************************************************************************/
StorageFile::StorageFile(const char* stor_file_path, size_t size, bool dbug_mode):
m_stor_file_path(stor_file_path), m_dbug_mode(dbug_mode)
{   
    // Enlarge size of file
    int fd = open(stor_file_path, O_RDWR);
    if (fd == -1) {
        throw std::runtime_error("Could not open file");
    }
    
    if (ftruncate(fd, size) == -1) {
        throw std::runtime_error("Failed in build storage file, check errno for details");
    }
}

} // namespace nbd
} // namespace ilrd
