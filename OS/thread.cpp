#include <iostream> // std
#include <string> // std

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

std::mutex mx;
std::condition_variable cv;

void Baz()
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    cout << "print something\n";
    cv.notify_one();
}

void Foo(const std::string& s)
{
    mx.lock();
    cout << "hello " << s << endl;
    mx.unlock();
}

struct Bar
{
    const std::string& m_name;
    Bar(const std::string& name=""): m_name(name){}

    void operator()()
    {
        mx.lock();
        cout << "hello " << m_name << endl;
        mx.unlock();
    }
};


int main() {

    std::thread thr_arr[7];
    
    for (int i = 0; i < 3; ++i){
        thr_arr[i] = std::thread(Foo, "yaron");
    }
    
    for (int i = 3; i < 6; ++i){
        thr_arr[i] = std::thread(Bar("moran")); //temp obj
    }

    for (int i = 6; i < 7; ++i){
        thr_arr[i] = std::thread([](){
                                        cout << "bla" << endl;
                                     });
    }

    for (auto& th : thr_arr){
        th.join();
    }
    
    thread thr1(Baz);
    thread thr2(Baz);
    thread thr3(Baz);
    
    //wait for threads to finish
    std::unique_lock<std::mutex> locker(mx);
    cv.wait(locker);
    locker.unlock();
    
    cout << "Back in main\n";
    
    thr1.detach();
    thr2.detach();
    thr3.detach();
    
    //~ thr1.join();
    //~ thr2.join();
    //~ thr3.join();
    
    return 0;
}
