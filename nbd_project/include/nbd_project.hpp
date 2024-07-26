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
#include "storage_file.hpp"

/******************************************************************************/
/*                                 ilrd                                       */  
/******************************************************************************/
namespace ilrd{
namespace nbd {

/******************************************************************************/
/*                          NBD Global Functions                              */  
/******************************************************************************/
void read_all(int fd, char* buf, size_t count);
void write_all(int fd, char* buf, size_t count);
u_int64_t ntohll(u_int64_t a) noexcept;

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
/*                          BDproxy Class                                     */  
/******************************************************************************/
class BDproxy {
public:
    explicit BDproxy(const char* dev_file, size_t dev_size, const char* stor_file_path, 
                     const char* monitor_path); //throw runtime_error
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
 
    StorageFile m_storage_file;
    std::string m_monitor_path;
};

/******************************************************************************/
/*                      BDproxy Class Implementation                          */  
/******************************************************************************/
inline int* BDproxy::get_fd() noexcept
{
    return m_fd;
}

inline bool BDproxy::get_staus() noexcept
{
    return m_conected_status;
}

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

} // namespace nbd
} // namespace ilrd

#endif // BUSE_HPP_
