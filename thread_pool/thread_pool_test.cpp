#include <iostream> // std

#include <unistd.h> 

#include "thread_pool.hpp"

using namespace std;
using namespace ilrd;

struct MyFunc1
{
    void operator()(unsigned int& x) const
    {
        sleep(1);
        cerr << "x = " << x << endl; ;
    }
};

struct MyFunc2
{
    void operator()(string& str1, string& str2) const
    {
        sleep(1);
        cerr << "string: " << str1 << " " << str2 << endl;
    }
};

int main()
{
    ThreadPool pool(3);

    MyFunc1 to_do1;
    MyFunc2 to_do2;
    
    unsigned int x = 5;
    pool.add_task<MyFunc1, unsigned int>(ThreadPool::Priority::LOW, to_do1, x);
    pool.add_task<MyFunc1, unsigned int>(ThreadPool::Priority::LOW, to_do1, x); 
    pool.add_task<MyFunc1, unsigned int>(ThreadPool::Priority::LOW, to_do1, x); 
    pool.add_task<MyFunc1, unsigned int>(ThreadPool::Priority::LOW, to_do1, x); 
    pool.add_task<MyFunc1, unsigned int>(ThreadPool::Priority::LOW, to_do1, x); 
    pool.add_task<MyFunc1, unsigned int>(ThreadPool::Priority::LOW, to_do1, x); 
    pool.add_task<MyFunc1, unsigned int>(ThreadPool::Priority::LOW, to_do1, x);
    string s1 = "Yaron";
    string s2 = "Zehavi";
    pool.add_task<MyFunc2, string, string>(ThreadPool::Priority::HIGH, to_do2, s1, s2);

    sleep(7);
    cout << "Num of threads(3): " << pool.get_num_threads() << endl;
    
    pool.set_num_threads(20);
    sleep(2);
    cout << "Set num of threads(20): " << pool.get_num_threads() << endl;

    pool.set_num_threads(5);
    sleep(2);
    cout << "Set num of threads(5): " << pool.get_num_threads() << endl;

    cout << pool.stop(5) << endl;

    return 0;
}
