#ifndef HANDLETON_HPP_
#define HANDLETON_HPP_

#include <iostream> // cout
#include <atomic>   // atomic

namespace ilrd{

template <class T>
class Handleton{
public:
    static T* get_instance();
    static void del_instance();
   ~Handleton() = default;
    Handleton(const Handleton& other) = delete;
    Handleton& operator=(Handleton other) = delete;
private:
    static std::atomic<T*> m_instance;
    static std::atomic<bool> m_is_del;
    static std::atomic<bool> m_lock;
    Handleton();
};

template <class T>
std::atomic<T*> Handleton<T>::m_instance(nullptr);
template <class T>
std::atomic<bool> Handleton<T>::m_is_del(false);
template <class T>
std::atomic<bool> Handleton<T>::m_lock(false);
}

#define DEFINE_HANDLETON(T) \
namespace ilrd{ \
template <> \
class Handleton<T>{ \
public: \
    static T* get_instance(); \
    static void del_instance(); \
   ~Handleton() = default; \
    Handleton(const Handleton& other) = delete; \
    Handleton& operator=(Handleton other) = delete; \
private: \
    static std::atomic<T*> m_instance; \
    static std::atomic<bool> m_is_del; \
    static std::atomic<bool> m_lock; \
    Handleton(); \
}; \
std::atomic<T*> Handleton<T>::m_instance(nullptr); \
std::atomic<bool> Handleton<T>::m_is_del(false); \
std::atomic<bool> Handleton<T>::m_lock(false); \
\
void Handleton<T>::del_instance() \
{ \
    bool is_del = m_is_del.load(std::memory_order_acquire); \
    if(!is_del){ \
        std::atomic_thread_fence(std::memory_order_acquire); \
        T* instance = m_instance.load(std::memory_order_relaxed); \
        delete instance; instance = nullptr; \
        m_instance.store(instance, std::memory_order_relaxed); \
        m_is_del.store(true, std::memory_order_relaxed); \
        std::atomic_thread_fence(std::memory_order_release); \
    } \
} \
T* Handleton<T>::get_instance() \
{ \
    bool is_del = m_is_del.load(std::memory_order_acquire); \
    if(!is_del){ \
        std::atomic_thread_fence(std::memory_order_acquire); \
        T* instance = m_instance.load(std::memory_order_relaxed); \
        if(instance == nullptr){ \
            while(m_lock.exchange(true, std::memory_order_relaxed)) \
                ; \
            instance = m_instance.load(std::memory_order_relaxed); \
            if(instance == nullptr) { \
                instance = new T; \
                m_instance.store(instance, std::memory_order_relaxed); \
                atexit(del_instance); \
            } \
            m_lock.store(false, std::memory_order_relaxed); \
        } \
        std::atomic_thread_fence(std::memory_order_release); \
        return instance; \
    } \
    else{    \
        std::cout << "erased" << std::endl; \
        abort(); \
    } \
} \
} \

#endif // HANDLETON_HPP_
