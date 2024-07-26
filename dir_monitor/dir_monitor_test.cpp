#include <iostream> // std
#include <unistd.h> // sleep

#include "dir_monitor.hpp"

#define NUM_OF_OBSERVERS 1

using namespace std;
using namespace ilrd;

int counter = 0;

class Observe {
public:
    Observe() : m_id_num(++counter){}
    
    void notify(MonitorEvent event)
    {
        cout << " File: " << event.m_file_name << " event: " << event.m_event <<  endl;
    }
    
    void dead_notify()
    {
        cout << " id " << m_id_num << " event: DEAD" << endl;
    }
    
private:
    int m_id_num;
};

int main(int argc, char* argv[])
{
	if (argc < 2){
		cout << "pathname...\n" << argv[0] << endl;
		exit(1);
	}

    DirMonitor monitor(argv[1], IN_CLOSE_WRITE);  // IN_OPEN | IN_ISDIR
    Observe ob[NUM_OF_OBSERVERS];
    CallBack<MonitorEvent, Observe>* ptr[NUM_OF_OBSERVERS];

	for (int i = 0; i < NUM_OF_OBSERVERS; i++) {
		ptr[i] = new CallBack<MonitorEvent,Observe>(&ob[i], &Observe::notify, &Observe::dead_notify);
		monitor.subscribe(ptr[i]);
	}
	
	monitor.run();    
    
    sleep(1000);

    for (int i = 0; i < NUM_OF_OBSERVERS; i++) {
        delete ptr[i];
    }
}
