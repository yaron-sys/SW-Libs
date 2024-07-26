#ifndef STORGAE_FILE_HPP_
#define STORGAE_FILE_HPP_

#include <stdexcept>    //exception

#include "nbd_project.hpp"  //nbd

namespace ilrd{
namespace nbd {

/******************************************************************************/
/*                          StorageFile Class                                 */  
/******************************************************************************/    
class StorageFile {
public:
    explicit StorageFile(const char* stor_file_path, size_t size); //throw runtile_error
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
};

/******************************************************************************/
/*                       StorageFile Class  Implementation                    */  
/******************************************************************************/
inline StorageFile::StorageFile(const char* stor_file_path, size_t size): m_stor_file_path(stor_file_path)
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

/******************************************************************************/
inline int StorageFile::read(void *buf, u_int32_t len, u_int64_t offset)
{
#ifndef NDEBUG
    std::cerr << "R - " << offset << ", " << len << std::endl;
#endif
    std::ifstream is(m_stor_file_path, std::ios::out); 
    is.seekg(offset, is.beg);
    is.read((char*)buf, len);
    is.close();
    
    return 0;
}

/******************************************************************************/
inline int StorageFile::write(const void *buf, u_int32_t len, u_int64_t offset)
{
#ifndef NDEBUG
    std::cerr << "W - " << offset << ", " << len << std::endl;
#endif
    
    std::ofstream os(m_stor_file_path, std::ios::in);
    os.seekp(offset, os.beg);
    os.write((char*)buf, len);
    os.close();
    
    return 0;
}

/******************************************************************************/
inline int StorageFile::disc()
{
#ifndef NDEBUG
    std::cerr << "Received a disconnect request." << std::endl;
#endif
    
    return 0;
}
/******************************************************************************/
inline int StorageFile::flush()
{
#ifndef NDEBUG
    std::cerr << "Received a flush request." << std::endl;
#endif
    
    return 0;
}

/******************************************************************************/
inline int StorageFile::trim(u_int64_t from, u_int32_t len)
{
#ifndef NDEBUG
    std::cerr << "T - " << from << ", " << len << std::endl;
#endif
    
    return 0;
}


} // namespace nbd
} // namespace ilrd

#endif // STORGAE_FILE_HPP_
