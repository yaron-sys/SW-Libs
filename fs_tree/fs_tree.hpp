#ifndef FS_TREE_HPP_
#define FS_TREE_HPP_

#include <string>      // string
#include <vector>      // vector

#include "util.hpp"    // macros

namespace ilrd{
namespace fs_tree{

class Entry{
public:
    virtual ~Entry() = default;
    
    Entry(const Entry& other) = delete;
    Entry& operator=(const Entry& other) = delete;
    
    const std::string& get_entry_name() const;
    virtual void print() const = 0;
    
    long m_depth_factor;
    
protected:
    Entry(const std::string& path_to_entry, long depth); 
    const std::string m_entry_name; 
}; 

class Dir: public Entry{
public:
    Dir(const std::string& path_to_entry, long depth);
   ~Dir() noexcept;
    
    Dir(const Dir& other) = delete;
    Dir& operator=(const Dir& other) = delete;

    void add(Entry* entry);
    void print() const;
    
    static long s_dir_count;
    
private:
    std::vector<Entry*> m_content;
}; 

class File: public Entry{
public:
    File(const std::string& path_to_entry, long depth);
   ~File() = default;
    
    File(const File& other) = delete;
    File& operator=(const File& other) = delete;
    void print() const;
    
    static long s_file_count;
}; 

}  // namespace fs_tree
}  // namespace ilrd

#endif // FS_TREE_HPP_
