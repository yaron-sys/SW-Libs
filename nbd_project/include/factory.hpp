#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include <stdexcept>     // runtime_error
#include <unordered_map> // unordered_map
#include <memory>        // uniquie_ptr
#include <cassert>       // assert 
	
namespace ilrd {
    
template <class Base, class Key, class Args>
class Factory {
public: 
    Factory() = default;
   ~Factory() = default;
    Factory(const Factory& other) = delete;
    Factory& operator=(const Factory& other) = delete;

    std::unique_ptr<Base> create(const Key& key, const Args& args) const; // throws invaild key
    void add(const Key& key, Base*(*func)(const Args& args));             // throws key already exists 
    void remove(const Key& key);                                          // throws invaild key

    struct FactoryException : public std::runtime_error {
        FactoryException(const std::string& s = "unknown error"): runtime_error(s){}
    }; 
    
private:
    std::unordered_map<Key, Base*(*)(const Args& args)> m_map;
};


template <class Base, class Key, class Args>
inline std::unique_ptr<Base> Factory<Base,Key,Args>::create(const Key& key, const Args& args) const
{
    auto itr = m_map.find(key);
    if (itr == m_map.end()){
        throw Factory::FactoryException("Invalid Key in create");
    }
    
    std::unique_ptr<Base> base((itr->second(args)));
    
    return base;
}

template <class Base, class Key, class Args>
inline void Factory<Base,Key,Args>::add(const Key& key, Base*(*func)(const Args& args))
{
    assert(func != nullptr);
    
    if (m_map.find(key) != m_map.end()){
        throw Factory::FactoryException("Key Already Exists");
    }
    m_map[key] = func;
}

template <class Base, class Key, class Args>
inline void Factory<Base,Key,Args>::remove(const Key& key)
{
    if (m_map.find(key) == m_map.end()){
        throw Factory::FactoryException("Invalid Key in remove");
    }
    
    m_map.erase(key);
}


} // namespace ilrd

#endif // FACTORY_HPP_
