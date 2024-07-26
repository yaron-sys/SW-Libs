#ifndef DIR_MONITOR_HPP_
#define DIR_MONITOR_HPP_

#include <cstddef> 		 // size_t
#include <iostream>		 // cout
#include <string>		 // string
#include <stdexcept>     // runtime_error
#include <thread>		 // thread

#include <sys/inotify.h> // inotify
#include <climits>		 // NAME_MAX 
#include <unistd.h>		 // read

#include "callback.hpp"  // callback

namespace ilrd{

struct MonitorEvent {
	MonitorEvent(const std::string& event, const std::string& file_name): m_event(event), m_file_name(file_name) {}
	std::string m_event;
	std::string m_file_name;
};

class DirMonitor{
public:
	DirMonitor(const std::string& path, uint32_t flags = IN_ALL_EVENTS);
   ~DirMonitor();
	void subscribe(CallBackBase<MonitorEvent>* viewer);
	void run();

private:
	Dispatcher<MonitorEvent> m_disp;
	int m_inotifyFd;
	struct inotify_event *m_event;
	char m_buf[10 * sizeof(struct inotify_event) + NAME_MAX + 1];
	
	static inline size_t buf_len()
    {
        return 10 * sizeof(struct inotify_event) + NAME_MAX + 1;
    }
    
    void start_monitor();
    void displayInotifyEvent();
};

// Display information from inotify_event structure 
void DirMonitor::displayInotifyEvent()
{
	std::string cur_event;
	if (m_event->mask & IN_ACCESS)        cur_event ="IN_ACCESS ";
	if (m_event->mask & IN_ATTRIB)        cur_event ="IN_ATTRIB ";
	if (m_event->mask & IN_CLOSE_NOWRITE) cur_event ="IN_CLOSE_NOWRITE ";
	if (m_event->mask & IN_CLOSE_WRITE)   cur_event ="IN_CLOSE_WRITE ";
	if (m_event->mask & IN_CREATE)        cur_event ="IN_CREATE ";
	if (m_event->mask & IN_DELETE)        cur_event ="IN_DELETE ";
	if (m_event->mask & IN_DELETE_SELF)   cur_event ="IN_DELETE_SELF ";
	if (m_event->mask & IN_IGNORED)       cur_event ="IN_IGNORED ";
	if (m_event->mask & IN_ISDIR)         cur_event ="IN_ISDIR ";
	if (m_event->mask & IN_MODIFY)        cur_event ="IN_MODIFY ";
	if (m_event->mask & IN_MOVE_SELF)     cur_event ="IN_MOVE_SELF ";
	if (m_event->mask & IN_MOVED_FROM)    cur_event ="IN_MOVED_FROM ";
	if (m_event->mask & IN_MOVED_TO)      cur_event ="IN_MOVED_TO ";
	if (m_event->mask & IN_OPEN)          cur_event ="IN_OPEN ";
	if (m_event->mask & IN_Q_OVERFLOW)    cur_event ="IN_Q_OVERFLOW ";
	if (m_event->mask & IN_UNMOUNT)       cur_event ="IN_UNMOUNT ";

	if (m_event->len > 0) {
		m_disp.notify(MonitorEvent(cur_event, m_event->name));
	}
}

DirMonitor::DirMonitor(const std::string& path, uint32_t flags): m_event(nullptr)
{
	m_inotifyFd = inotify_init();
	if (m_inotifyFd == -1) {
		throw std::runtime_error("inotify_init");
	}
	
	if (inotify_add_watch(m_inotifyFd, path.c_str(), flags) == -1) {
		throw std::runtime_error("inotify_add_watch");
	}
	
#ifndef NDEBUG
    std::cout << "Watching " << path << std::endl;
#endif
}

DirMonitor::~DirMonitor()
{
	close(m_inotifyFd);
}

void DirMonitor::start_monitor()
{
	while (true) {                           
		ssize_t numRead = read(m_inotifyFd, m_buf, buf_len());
		if (numRead == 0) {
			throw std::runtime_error("read() from inotify fd returned 0!");
		}

		if (numRead == -1) {
			throw std::runtime_error("read failed");
		}

#ifndef NDEBUG
		std::cout << "Read " << (long) numRead << "bytes from inotify fd." << std::endl;
#endif

		// Process all of the events in buffer returned by read() 
		char* p = m_buf;
		while (p < m_buf + numRead) {
			m_event = (struct inotify_event*) p;
			displayInotifyEvent();
			p += sizeof(struct inotify_event) + m_event->len;
		}
	}
}

void DirMonitor::run()
{
	std::thread run_thread(&DirMonitor::start_monitor, this);
	run_thread.detach();
}

void DirMonitor::subscribe(CallBackBase<MonitorEvent>* viewer)
{
	m_disp.subscribe(viewer);
}

} // namespace ilrd

#endif // DIR_MONITOR_HPP_
