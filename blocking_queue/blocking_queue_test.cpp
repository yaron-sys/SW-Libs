#include <iostream>
#include <thread>

#include "blocking_queue.hpp"

using namespace std;
using namespace ilrd;

int main()
{
    BlockingQueue<int> bq;
    int value_out = 0;
    
    std::thread thr_arr[3];
   
    thr_arr[0] = std::thread(&BlockingQueue<int>::push, &bq, 20);
    
    
    for (int i = 1; i < 3; ++i){
        thr_arr[i] = std::thread(&BlockingQueue<int>::try_pop_for, &bq, &value_out, 2000);
    }
    
    for (auto& th : thr_arr){
        th.join();
    }
    
    cout << "value: " << value_out << endl;
    
    return 0;
}
