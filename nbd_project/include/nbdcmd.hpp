#ifndef NBDCMD_HPP_
#define NBDCMD_HPP_

#include <stdexcept>    // exception

#include "nbd_project.hpp"  // nbd

namespace ilrd{
namespace nbd {

class StorageFile;

struct Args {
    u_int32_t m_len;
    u_int64_t m_offset;
    bool m_dbug_mode;
    struct nbd_reply m_reply;
    StorageFile* m_storage_file;
    int m_sk;
};

class NBDCommand{
public:
    NBDCommand(const Args& args);
    virtual ~NBDCommand() = default;
    virtual void do_it() = 0;
    virtual void* get_buf() {return 0;}
protected:
    Args nbd_args;
};




} // namespace nbd
} // namespace ilrd

#endif // NBDCMD_HPP_
