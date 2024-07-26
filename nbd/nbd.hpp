/******************************************************************************/
/*                          C++  - BUSE - NBD                                 */ 
/******************************************************************************/
#ifndef BUSE_HPP_
#define BUSE_HPP_

#include <iostream>
#include <thread>       //thread
#include <stdexcept>    //exception
#include <cstring>      //memcpy

#include <assert.h>     //assert
#include <linux/nbd.h>  //nbd
#include <netinet/in.h> //htonl
#include <sys/ioctl.h>  //ioctl
#include <sys/socket.h> //socketpair
#include <unistd.h>     //read, write

#include <sys/stat.h>   //fstat
#include <fcntl.h>      //open permission
#include <sys/mman.h>   //mmap
#include <fstream>      // ifstream / ostream

#include "nbdcmd.hpp"	//nbd command base class

/******************************************************************************/
/*                                 ilrd                                       */  
/******************************************************************************/
namespace ilrd{
namespace nbd {

/******************************************************************************/
/*                          NBD Function                                      */  
/******************************************************************************/
void read_all(int fd, char* buf, size_t count);
void write_all(int fd, char* buf, size_t count);
u_int64_t ntohll(u_int64_t a) noexcept;

/******************************************************************************/
/*                          StorageFile Class                                 */  
/******************************************************************************/    
class StorageFile {
public:
    explicit StorageFile(const char* stor_file_path, size_t size, bool dbug_mode); //throw runtile_error
   ~StorageFile() = default;
    StorageFile(const StorageFile& other) = delete;
    StorageFile& operator=(const StorageFile& other) = delete;

    //methods print error to stderr(runing in thread)
    int read(void *buf, u_int32_t len, u_int64_t offset);
    int write(const void *buf, u_int32_t len, u_int64_t offset);
    int disc();
    int flush();
    int trim(u_int64_t from, u_int32_t len);

private:
    const char* m_stor_file_path;
    bool m_dbug_mode;
};
    
/******************************************************************************/
/*                          NBD Commands                                      */  
/******************************************************************************/
class Read: public NBDCommand{
public:    
    Read(const Args& args);
   ~Read();
    void do_it();
private:
    void *buf;
};

class Write: public NBDCommand{
public:    
    Write(const Args& args);
   ~Write();
    void do_it();
    void* get_buf();
private:
    void *buf;
};

class Disc: public NBDCommand{
public:    
    Disc(const Args& args);
   ~Disc() = default;
    void do_it();
};

class Flush: public NBDCommand{
public:    
    Flush(const Args& args);
   ~Flush() = default;
    void do_it();
};

class Trim: public NBDCommand{
public:    
    Trim(const Args& args);
   ~Trim() = default;
    void do_it();
};

/******************************************************************************
                            Methods                   
******************************************************************************/
inline NBDCommand::NBDCommand(const Args& args)
{
    nbd_args.m_len = args.m_len;
    nbd_args.m_offset = args.m_offset;  
    nbd_args.m_dbug_mode = args.m_dbug_mode;
    nbd_args.m_reply = args.m_reply;
    nbd_args.m_storage_file = args.m_storage_file;
    nbd_args.m_sk = args.m_sk;
}

/******************************************************************************/
inline Disc::Disc(const Args& args): NBDCommand(args)
{}

inline void Disc::do_it()
{
    nbd_args.m_storage_file->disc();
}
/******************************************************************************/
inline Flush::Flush(const Args& args): NBDCommand(args)
{}

inline void Flush::do_it()
{
#ifdef NBD_FLAG_SEND_FLUSH
    nbd_args.m_reply.error = nbd_args.m_storage_file->flush();
    write_all(nbd_args.m_sk, (char*)&nbd_args.m_reply, sizeof(nbd_args.m_reply));
#endif
}
/******************************************************************************/
inline Trim::Trim(const Args& args): NBDCommand(args)
{}

inline void Trim::do_it()
{
#ifdef NBD_FLAG_SEND_TRIM
    nbd_args.m_reply.error = nbd_args.m_storage_file->trim(nbd_args.m_offset, nbd_args.m_len);
    write_all(nbd_args.m_sk, (char*)&nbd_args.m_reply, sizeof(nbd_args.m_reply));
#endif
}

/******************************************************************************/
/*                          BDproxy Class                                     */  
/******************************************************************************/
class BDproxy {
public:
    explicit BDproxy(const char* dev_file, size_t dev_size, const char* stor_file_path, 
                     const char* monitor_path, bool dbug_mode); //throw runtime_error
   ~BDproxy();
    BDproxy(const BDproxy& other) = delete;
    BDproxy& operator=(const BDproxy& other) = delete;
    
    int* get_fd() noexcept;
    bool get_staus() noexcept;
    
private:
    void run_nbd(int nbd);
    void run_rw();
    
    std::thread m_do_it; 
    std::thread m_rw;
    
    int m_fd[2];
    const char* m_dev_file;
    bool m_conected_status;
    bool m_dbug_mode;
    
    StorageFile m_storage_file;
    std::string m_monitor_path;
};

/******************************************************************************/
/*                          NBD Function                                      */  
/******************************************************************************/
#ifdef WORDS_BIGENDIAN
inline u_int64_t ntohll(u_int64_t a) noexcept
{
    return a;
}
#else
inline u_int64_t ntohll(u_int64_t a) noexcept 
{
    u_int32_t lo = a & 0xffffffff;
    u_int32_t hi = a >> 32U;
    lo = ntohl(lo);
    hi = ntohl(hi);
    return ((u_int64_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll

/******************************************************************************/
/*                      BDproxy Class  Implementation                         */  
/******************************************************************************/
/******************************************************************************
                                    Methods                  
******************************************************************************/
inline int* BDproxy::get_fd() noexcept
{
    return m_fd;
}

inline bool BDproxy::get_staus() noexcept
{
    return m_conected_status;
}

/******************************************************************************/
/*                       StorageFile Class  Implementation                    */  
/******************************************************************************/
/******************************************************************************
                            Methods                   
******************************************************************************/
inline int StorageFile::read(void *buf, u_int32_t len, u_int64_t offset)
{
    if (m_dbug_mode) {
        std::cerr << "R - " << offset << ", " << len << std::endl;
    }
    std::ifstream is(m_stor_file_path, std::ios::out); 
    is.seekg(offset, is.beg);
    is.read((char*)buf, len);
    is.close();
    
    return 0;
}

/******************************************************************************/
inline int StorageFile::write(const void *buf, u_int32_t len, u_int64_t offset)
{
    if (m_dbug_mode) {
        std::cerr << "W - " << offset << ", " << len << std::endl;
    }
    
    std::ofstream os(m_stor_file_path, std::ios::in);
    os.seekp(offset, os.beg);
    os.write((char*)buf, len);
    os.close();
    
    return 0;
}

/******************************************************************************/
inline int StorageFile::disc()
{
    if (m_dbug_mode) {
        std::cerr << "Received a disconnect request." << std::endl;
    }
    
    return 0;
}
/******************************************************************************/
inline int StorageFile::flush()
{
    if (m_dbug_mode) {
        std::cerr << "Received a flush request." << std::endl;
    }
    
    return 0;
}

/******************************************************************************/
inline int StorageFile::trim(u_int64_t from, u_int32_t len)
{
    if (m_dbug_mode) {
        std::cerr << "T - " << from << ", " << len << std::endl;
    }
    
    return 0;
}

} // namespace nbd
} // namespace ilrd

#endif // BUSE_HPP_
