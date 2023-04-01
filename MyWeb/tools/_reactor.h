#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "_channel.h"

namespace dya
{
////////////////////////////////////////////////////////////////////
class EpollPoller
{
public :
	EpollPoller();
	~EpollPoller();
	//LT(水平触发);ET(边沿触发)
    void addChannel(Channel *ch);
    void loop();
    void closeLoop();
private :
    std::vector<Channel*> wait(int timeout = -1);
private :
	int m_epfd;
    std::unique_ptr<epoll_event[]> m_waitResult;
    bool m_enable;
};
////////////////////////////////////////////////////////////////////
class ThreadPool;
class AcceptChannel;

class Reactor
{
    friend dya::AcceptChannel;
public :
    Reactor(int port);
    Reactor(int port, int threadCount);
    ~Reactor();

    template<class Fn, class... Args>
    void setParse(Fn&& fn, Args&&... args);

    void loop();
private :
    std::string defaultParse(std::string&);
private :
    AcceptChannel m_acceptChannel;
    std::unique_ptr<EpollPoller> m_master;
    std::unique_ptr<EpollPoller[]> m_slaves;
    std::unique_ptr<ThreadPool> m_threadPool;
};

template<class Fn, class... Args>
void Reactor::setParse(Fn&& fn, Args&&... args)
{
    m_acceptChannel.setHandle(std::bind(std::forward<Fn>(fn), std::forward<Args>(args)..., std::placeholders::_1));
}
////////////////////////////////////////////////////////////////////
}