#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include <iostream> // cout
#include <atomic>   // atomic

#include "utils.hpp" // macros

namespace ilrd{

template <class T>
class Singleton{
public:
    static T* get_instance();
    static void del_instance();
   ~Singleton() = default;
    
    Singleton(const Singleton& other) = delete;
    Singleton& operator=(Singleton other) = delete;

private:
    static std::atomic<T*> m_instance;
    static std::atomic<bool> m_is_del;
    static std::atomic<bool> m_lock;
    
    Singleton();
};

template <class T>
std::atomic<T*> Singleton<T>::m_instance(nullptr);
template <class T>
std::atomic<bool> Singleton<T>::m_is_del(false);
template <class T>
std::atomic<bool> Singleton<T>::m_lock(false);


template <class T>
T* Singleton<T>::get_instance()
{
    bool is_del = m_is_del.load(std::memory_order_acquire);
    
    if(!is_del){
        std::atomic_thread_fence(std::memory_order_acquire);
        T* instance = m_instance.load(std::memory_order_relaxed);
        
        if(instance == nullptr){
            // busy wait
            while(m_lock.exchange(true, std::memory_order_relaxed))
                ;
            instance = m_instance.load(std::memory_order_relaxed);
            if(instance == nullptr) {
                instance = new T;
                m_instance.store(instance, std::memory_order_relaxed);
                atexit(del_instance); // set atexit
            }
            m_lock.store(false, std::memory_order_relaxed);
            // end busy wait
        }
        
        std::atomic_thread_fence(std::memory_order_release);
        return instance;
    }

    else{   
        std::cout << "erased" << std::endl;
        abort();
    }
}

template <class T>
void Singleton<T>::del_instance()
{
    bool is_del = m_is_del.load(std::memory_order_acquire);
    
    if(!is_del){
        std::atomic_thread_fence(std::memory_order_acquire);
        
        T* instance = m_instance.load(std::memory_order_relaxed);
        delete instance; instance = nullptr;
        m_instance.store(instance, std::memory_order_relaxed);
        m_is_del.store(true, std::memory_order_relaxed);
        
        std::atomic_thread_fence(std::memory_order_release);
    }
}

} // namespace ilrd

#endif // SINGLETON_HPP_
