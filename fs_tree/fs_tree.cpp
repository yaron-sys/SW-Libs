#include <iostream>     // cout
#include <algorithm>    // lower_bound

#include "fs_tree.hpp"

// initialize static counters
long ilrd::fs_tree::File::s_file_count = 0;
long ilrd::fs_tree::Dir::s_dir_count = 0;

namespace ilrd{
namespace fs_tree{

/*******************************************************************************
                                    Entry
*******************************************************************************/
Entry::Entry(const std::string& path_to_entry, long depth): m_depth_factor(depth), 
                                                            m_entry_name(path_to_entry)
{}

const std::string& Entry::get_entry_name() const
{
    return m_entry_name;
}

/*******************************************************************************
                                  Directory
*******************************************************************************/
Dir::Dir(const std::string& path_to_entry, long depth): Entry(path_to_entry, depth)
{
    ++s_dir_count;
}

Dir::~Dir() noexcept
{
    for (Entry* obj : m_content){
        delete obj;
    }
    m_content.clear();
}

void Dir::add(Entry* entry)
{
    // add entry to sorted vector
    std::vector<Entry*>::iterator itr;
    
    auto comp = [](Entry* lhs, Entry* rhs)->bool
    {
        return lhs->get_entry_name().compare(rhs->get_entry_name()) <= 0;
    };
    
    itr = std::lower_bound(m_content.begin(), m_content.end(), entry, comp);
    
    // insert before iterator
    m_content.insert(itr, entry); 
}

void Dir::print() const
{    
    std::string ident_sign;
    
    for (long i = 0; i < m_depth_factor; ++i) {
        ident_sign += "│   ";
    }
    
    ident_sign += "├── ";
    
    std::cout << BOLD BLUE << m_entry_name << RESET <<std::endl;
    
    auto printobj = [&](Entry* obj)->void
    {
        std::cout << ident_sign;
        obj->print();
    };
    
    for_each(m_content.begin(), m_content.end(), printobj);
}

/*******************************************************************************
                                    FILE
*******************************************************************************/
File::File(const std::string& path_to_entry, long depth): Entry(path_to_entry, depth)
{
    ++s_file_count;
}

void File::print() const
{
    std::cout << m_entry_name << std::endl;
}
/******************************************************************************/

}  // namespace fs_tree
} // namespace ilrd
