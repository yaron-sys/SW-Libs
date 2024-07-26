#ifndef BLOCKING_QUEUE_HPP_
#define BLOCKING_QUEUE_HPP_

#include <queue>                // std::priority_queue
#include <mutex>                // mutex
#include <condition_variable>   // condition_variable
#include <vector>               // vector
#include <chrono>
#include <cassert>

namespace ilrd {

template <class Type, class Container = std::vector<Type>, class Compare = std::less<Type>>
class BlockingQueue {
public:
    BlockingQueue() = default;
   ~BlockingQueue() = default;
    BlockingQueue(const BlockingQueue& other) = delete;
    BlockingQueue& operator=(const BlockingQueue& other) = delete;
    
    void push(const Type& item);                        // throws ...
    void pop(Type* out);                                // may block, throws...
    bool try_pop_for(Type* out, int ms_timeout = 1000); // timeout in milliseconds
    
    bool empty() const;
    size_t size() const;
    void clear();
    
private:
    std::priority_queue<Type, Container, Compare> m_queue;
    mutable std::mutex m_mtx;
    mutable std::condition_variable m_cv;
};

  
/*******************************************************************************
                          BlockingQueue Methods          
*******************************************************************************/
template <class Type, class Container, class Compare>
void BlockingQueue<Type,Container,Compare>::push(const Type& item)
{
    std::unique_lock<std::mutex> locker(m_mtx);
    m_queue.push(item);
    m_cv.notify_one();
}

template <class Type, class Container, class Compare>
void BlockingQueue<Type,Container,Compare>::pop(Type* out)
{
    std::unique_lock<std::mutex> locker(m_mtx);
    m_cv.wait(locker, [this](){ return !m_queue.empty(); } );
    assert(out != nullptr);
    *out = m_queue.top();
    m_queue.pop();
}

template <class Type, class Container, class Compare>
bool BlockingQueue<Type,Container,Compare>::try_pop_for(Type* out, int ms_timeout)
{
    std::unique_lock<std::mutex> locker(m_mtx);
    if(m_cv.wait_for(locker, static_cast<std::chrono::milliseconds>(ms_timeout), [this](){ return !m_queue.empty(); })){
        assert(out != nullptr);
        *out = m_queue.top();
        m_queue.pop();
        return true;
    }
    return false;
}

template <class Type, class Container, class Compare>
bool BlockingQueue<Type,Container,Compare>::empty() const
{
    std::unique_lock<std::mutex> locker(m_mtx);
    return m_queue.empty();
}

template <class Type, class Container, class Compare>
size_t BlockingQueue<Type,Container,Compare>::size() const
{
    std::unique_lock<std::mutex> locker(m_mtx);
    return m_queue.size();
}
    
template <class Type, class Container, class Compare>    
void BlockingQueue<Type,Container,Compare>::clear()
{
    std::unique_lock<std::mutex> locker(m_mtx);
    while(!m_queue.empty()){
        m_queue.pop();
    }
}

} // namespace ilrd
            
#endif // BLOCKING_QUEUE_HPP_
