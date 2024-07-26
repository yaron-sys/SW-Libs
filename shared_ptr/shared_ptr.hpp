#ifndef SHARED_PTR_HPP_
#define SHARED_PTR_HPP_

#include <atomic>       // atomic
#include <algorithm>    // swap

#include "util.hpp"     // macros

namespace ilrd{

namespace SharedPtr_Details{
    class RefCounter {
    public:
        RefCounter(long counter);
        long get_count() const;
        void increment();
        void decrement();

    private:
        std::atomic<long> m_c_counter;
    };
} // SharedPtr_Details

template <class T>
class SharedPtr;

template <class T>
bool operator==(const T* lhs, const SharedPtr<T>& rhs);

template <class T>
bool operator==(const SharedPtr<T>& lhs, const T* rhs);

template <class T>
bool operator==(const SharedPtr<T>& lhs, const SharedPtr<T>& rhs);

template <class T>
bool operator!=(const T* lhs, const SharedPtr<T>& rhs);

template <class T>
bool operator!=(const SharedPtr<T>& lhs, const T* rhs);

template <class T>
bool operator!=(const SharedPtr<T>& lhs, const SharedPtr<T>& rhs);

template <class T>
class SharedPtr {
public:
    explicit SharedPtr(T* ptr = nullptr);  // ptr must be allocated only by "new". no "delete" needed.
   ~SharedPtr();
    SharedPtr(const SharedPtr& other);
    SharedPtr& operator=(SharedPtr other);
    
    template<class U>
    SharedPtr(const SharedPtr<U>& other);

    T& operator*() const;
    T* operator->() const;

    explicit operator bool() const;

    T* get() const;

private:
    T* m_ptr;
    SharedPtr_Details::RefCounter* m_ref_count;
    
    template<class U> friend class SharedPtr;
};

/*******************************************************************************
 *                                  RefCounter
 ******************************************************************************/
inline SharedPtr_Details::RefCounter::RefCounter(long counter): m_c_counter(counter){}

inline long SharedPtr_Details::RefCounter::get_count() const
{
    return m_c_counter;
}

inline void SharedPtr_Details::RefCounter::increment()
{
    ++m_c_counter;
}

inline void SharedPtr_Details::RefCounter::decrement()
{
    --m_c_counter;
}

/*******************************************************************************
 *                                  SharedPtr
 ******************************************************************************/
template <class T>
inline SharedPtr<T>::SharedPtr(T* ptr)
{
    try{
        m_ref_count = new SharedPtr_Details::RefCounter(1);
    }
    catch(const std::bad_alloc&){
        delete ptr;
        throw;
    }
    m_ptr = ptr;
}


template <class T>
inline SharedPtr<T>::SharedPtr(const SharedPtr& other): m_ptr(other.m_ptr), 
                                                        m_ref_count(other.m_ref_count)
{
    m_ref_count->increment();
}

template <class T>
inline SharedPtr<T>::~SharedPtr()
{
    m_ref_count->decrement();
    if(m_ref_count->get_count() == 0){
        delete m_ptr;
        delete m_ref_count;
    }
}

template <class T>
inline SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr other)
{
    std::swap(m_ptr, other.m_ptr);
    std::swap(m_ref_count, other.m_ref_count);
    return *this;
}

template<class T>
template<class U>
inline SharedPtr<T>::SharedPtr(const SharedPtr<U>& other): m_ptr(other.m_ptr)
{
    m_ref_count = other.m_ref_count;
    m_ref_count->increment();
}

template <class T>
inline SharedPtr<T>::operator bool() const
{
    return (m_ptr != nullptr);
}

template <class T>
inline T& SharedPtr<T>::operator*() const
{
    return *m_ptr;
}
    
template <class T>
inline T* SharedPtr<T>::operator->() const
{
    return m_ptr;
}
    
template <class T>
inline T* SharedPtr<T>::get() const
{
    return m_ptr;
}

template <class T>
inline bool operator==(const T* lhs, const SharedPtr<T>& rhs)
{
    return lhs == rhs.get();
}

template <class T>
inline bool operator==(const SharedPtr<T>& lhs, const T* rhs)
{
    return lhs.get() == rhs;
}

template <class T>
inline bool operator==(const SharedPtr<T>& lhs, const SharedPtr<T>& rhs)
{
    return lhs.get() == rhs.get();
}

template <class T>
inline bool operator!=(const T* lhs, const SharedPtr<T>& rhs)
{
    return lhs != rhs.get();
}

template <class T>
inline bool operator!=(const SharedPtr<T>& lhs, const T* rhs)
{
    return lhs.get() != rhs;
}

template <class T>
inline bool operator!=(const SharedPtr<T>& lhs, const SharedPtr<T>& rhs)
{
    return lhs.get() != rhs.get();
}


} // namespace ilrd

#endif // SHARED_PTR_HPP_
