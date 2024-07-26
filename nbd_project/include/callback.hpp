#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <list> // list

namespace ilrd {
	
template <class Event>
class Dispatcher;

template <class Event>
class CallBackBase {
protected:
    CallBackBase();
    virtual ~CallBackBase();
    
private:
    CallBackBase(const CallBackBase& other) = delete;
	CallBackBase& operator=(const CallBackBase& other) = delete;
    
    virtual void notify(const Event& event) = 0;
	virtual void notify_death() = 0;
    
    friend class Dispatcher<Event>;
    
	Dispatcher<Event>* m_disp;
};

template <class Event, class Observer>
class CallBack: public CallBackBase<Event> {
public:
	using NotifyFunc = void (Observer::*)(Event);
    using DeathFunc = void (Observer::*)();
    
    CallBack(Observer* obs, NotifyFunc notify, DeathFunc death);
	virtual ~CallBack() = default;
	
private:
    CallBack(const CallBack& other) = delete;
	CallBack& operator=(const CallBack& other) = delete;
    
    virtual void notify(const Event& event) override;
	virtual void notify_death() override;
    
    Observer* m_obs;
	NotifyFunc m_notify_func;
	DeathFunc m_death_func;
};

template <class Event>
class Dispatcher final {
public:
	Dispatcher() = default;
   ~Dispatcher();
	
	void notify(Event event);
	void subscribe(CallBackBase<Event>* base);

private:
    Dispatcher(const Dispatcher& other) = delete;
	Dispatcher& operator=(const Dispatcher& other) = delete;
    
    friend class CallBackBase<Event>;
    
	void unsubscribe( CallBackBase<Event>* base);
    std::list<CallBackBase<Event>*> m_observers;
};

/*******************************************************************************
                             CallBackBase methods
*******************************************************************************/
template <class Event>
CallBackBase<Event>::CallBackBase(): m_disp(nullptr)
{}
    
template <class Event>
CallBackBase<Event>::~CallBackBase()
{
    if(m_disp != nullptr) {
        m_disp->unsubscribe(this);
    }
}

/*******************************************************************************
                             CallBack methods
*******************************************************************************/
template <class Event, class Observer>
CallBack<Event, Observer>::CallBack( Observer* obs, NotifyFunc notify, DeathFunc death)
: m_obs(obs), m_notify_func(notify), m_death_func(death)
{}

template <class Event, class Observer>
void CallBack<Event, Observer>::notify(const Event& event)
{
    (m_obs->*m_notify_func)(event);
}

template <class Event, class Observer>
void CallBack<Event, Observer>::notify_death()
{
    (m_obs->*m_death_func)();
}

/*******************************************************************************
                             Dispatcher methods
*******************************************************************************/
template <class Event>
Dispatcher<Event>::~Dispatcher()
{
    for(auto i : m_observers){
        i->notify_death();
        i->m_disp = nullptr;
    }
}

template <class Event>
void Dispatcher<Event>::notify(Event event)
{
    for(auto i : m_observers){
        i->notify(event);
    }
}

template <class Event>
void Dispatcher<Event>::subscribe(CallBackBase<Event>* base)
{
    m_observers.push_back(base);
    base->m_disp = this;
}

template <class Event>
void Dispatcher<Event>::unsubscribe(CallBackBase<Event>* base)
{
    m_observers.remove(base);
}

} // namespace ilrd 


#endif //OBSERVER_HPP
