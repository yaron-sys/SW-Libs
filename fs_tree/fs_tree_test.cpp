#include <iostream>     // cout
#include <dirent.h>     // dirent

#include "fs_tree.hpp"

namespace fs = ilrd::fs_tree;

namespace ilrd{
namespace fs_tree{
/*******************************************************************************
                                build_tree
*******************************************************************************/
Dir* build_tree(std::string path, Dir* dir_o = nullptr)
{
    struct dirent *entry;
    
    /* create a directory object */
    if (dir_o == nullptr) {
        dir_o = new Dir(path, -1);
    }
    
    dir_o->m_depth_factor += 1;
    
    DIR *dir = opendir(path.c_str());
    if (! dir) {
        std::cerr << "path not found:" << path << std::endl;
        return dir_o;
    }
    
    while ((entry = readdir(dir)) != nullptr) {
        
        std::string name = entry->d_name;
    
        if (entry->d_type == DT_DIR) {
            if (!name.compare(".") || !name.compare("..")) {
                continue;
            }
            
            Dir* sub_dir = new Dir(name, dir_o->m_depth_factor);
            dir_o->add(sub_dir);
            build_tree(path + '/' + name, sub_dir);
        } 
        else {
              /* create a file object */
              dir_o->add(new File(name, dir_o->m_depth_factor));
        }
    }    
    closedir(dir);
    
    return dir_o;
}

}  // namespace fs_tree
} // namespace ilrd


int main(int argc, char** argv)
{
    fs::Entry* tree = nullptr;
    std::string path = ((argc == 1) ? "." : argv[1]);

    try{
        tree = fs::build_tree(path);
    }
    catch(const std::bad_alloc&){
        delete tree;
        exit(1);
    }
    
    tree->print();
    std::cout << BOLD << std::endl << fs::Dir::s_dir_count-1 << " directories, " << 
                        fs::File::s_file_count << " files" << RESET << std::endl;

    delete tree;
    
    return 0;
}
