#include <iostream> // std

#include "callback.hpp"

using namespace std;
using namespace ilrd;

int counter = 0;

class Observe {
public:
    Observe() : m_id_num(++counter){}
    
    void notify(int event)
    {
        cout <<"id " << m_id_num << ":event- " << event << endl;
    }
    
    void dead_notify()
    {
        cout <<"id " << m_id_num << ":event- DEAD" << endl;
    }
    
private:
    int m_id_num;
};

int main()
{
    CallBack<int,Observe>* ptr[10];
    Observe ob[10];
    {
        Dispatcher<int> disp;
        
        for (int i = 0; i < 10; i++) {
        ptr[i] = new CallBack<int,Observe>(&ob[i], &Observe::notify, &Observe::dead_notify);
            disp.subscribe(ptr[i]);
        }
        
        {
            Observe *ob_ptr = new Observe;
            CallBack<int,Observe> observer1(ob_ptr, &Observe::notify, &Observe::dead_notify);
            disp.subscribe(&observer1);   
            disp.notify(10);
            delete ob_ptr;
        }
        
        disp.notify(9);
    }
    
    for (int i = 0; i < 10; i++) {
        delete ptr[i];
    }
    
}
