#ifndef THREAD_POOL_HPP_
#define THREAD_POOL_HPP_

#include <thread>
#include <cassert>
#include <list>
#include <atomic>
#include <condition_variable>

#include "blocking_queue.hpp"

namespace ilrd {

class ThreadPool {
public:
    ThreadPool(int n_threads = 6);
   ~ThreadPool() = default;
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;
    
    enum Priority { LOW, MED, HIGH, IMPL_DETAILS };
    
    template <class Func, class... Args>
    void add_task(Priority priority, Func task, Args&... args);
    
    int stop(int timeout); // may block, return number of threads not killed
    int get_num_threads() const;
    void set_num_threads(int n_threads);

private:
    using Task = std::pair<Priority, std::function<void(void)>>;
    using Thread_id = std::thread::id;
    using Container = std::vector<Task>;
    struct Compare {
        bool operator()(const Task& lhs, const Task& rhs) const noexcept;
    };
    
    void run();
    void del_threads();
    
    std::mutex m_mtx;
    std::condition_variable m_cv;
    std::atomic_bool m_should_stop;
    std::list<Thread_id> m_pool;
    
    int m_n_threads;
    BlockingQueue<Task, Container, Compare> m_tasks;
    static const int TRY_POP_THRESHOLD = 1000; // timeout in milliseconds
};

namespace ThreadPool_Detials {
    void poison_task(void) {}
}

/*******************************************************************************
                          Compare Method         
*******************************************************************************/
bool ThreadPool::Compare::operator()(const Task& lhs, const Task& rhs) const noexcept
{
    return lhs.first <= rhs.first;
}
/*******************************************************************************
                          ThreadPool Methods          
*******************************************************************************/           
ThreadPool::ThreadPool(int n_threads): m_should_stop(false), m_n_threads(0)
{
	set_num_threads(n_threads);
}

void ThreadPool::run()
{
    while (!m_should_stop){
        Task out;
        if (!m_tasks.try_pop_for(&out, ThreadPool::TRY_POP_THRESHOLD)) { continue; }
        if(out.first == IMPL_DETAILS) { break; }
        out.second(); // call task
    }
	
	std::unique_lock<std::mutex> locker(m_mtx);
    m_pool.remove_if([&](Thread_id& id) { return id == std::this_thread::get_id();});
    --m_n_threads;
    m_cv.notify_one();
}

int ThreadPool::stop(int timeout)
{
    m_should_stop = true;

    std::unique_lock<std::mutex> locker(m_mtx);
    m_cv.wait_for(locker, std::chrono::seconds(timeout), [&](){ return m_n_threads == 0; });
    m_tasks.clear();
    
    m_should_stop = false;
    
    return m_n_threads;
}

template <class Func, class... Args>
void ThreadPool::add_task(Priority priority, Func task, Args&... args)
{
    // create a task (pair of priority and function)
    std::function<void(void)> do_thing = [&](){ task(args...); };
    Task new_task(priority, do_thing);
    m_tasks.push(new_task); // push task to blocking queue
}

int ThreadPool::get_num_threads() const
{
    return m_n_threads;
}

void ThreadPool::set_num_threads(int n_threads)
{
    assert(n_threads >= 0);
    
    int threads_to_add = n_threads - m_n_threads;
    
    if (threads_to_add > 0) {
        // Add new threads to list
        for(int i = 0; i < threads_to_add; ++i){
			std::thread new_thread(&ThreadPool::run, this);
			m_pool.push_back(new_thread.get_id());
			new_thread.detach();
        }
        m_n_threads += threads_to_add;
    }
    
    else if (threads_to_add < 0) {
        // Kill correct amount of threads
        int threads_to_kill = -threads_to_add;
        
        for(int i = 0; i < threads_to_kill; ++i){
            add_task<void(void)>(ThreadPool::Priority::IMPL_DETAILS, ThreadPool_Detials::poison_task);
        }
    }
}


} // namespace ilrd
            
#endif // THREAD_POOL_HPP_
