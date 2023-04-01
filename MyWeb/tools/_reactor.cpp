#include "_reactor.h"
#include <unistd.h>
#include <cassert>
#include "_threadpool.h"

namespace dya
{
constexpr int kEVENT_MAX = 24;
////////////////////////////////////////////////////////////////////
EpollPoller::EpollPoller() : m_enable(true), m_waitResult(new epoll_event[kEVENT_MAX])
{
	m_epfd = epoll_create(1);
    assert(m_epfd != -1);
}

EpollPoller::~EpollPoller() 
{
    this->closeLoop();
	if (m_epfd != -1)
	{
		close(m_epfd);
		m_epfd = -1;
	}
}

void EpollPoller::addChannel(Channel *ch)
{
    struct epoll_event ev{0};
	//ev.events = ch->m_eventList;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = ch;
	epoll_ctl(m_epfd, EPOLL_CTL_ADD, ch->getFd(), &ev);  
}

std::vector<Channel*> EpollPoller::wait(int timeout)
{
	int count = epoll_wait(m_epfd, m_waitResult.get(), kEVENT_MAX, timeout);
	if(count == -1)
		return {};

	std::vector<Channel*> waitResult;
    Channel *temp = nullptr;
	for(int i=0; i<count; i++)
	{
        temp = (Channel*)m_waitResult[i].data.ptr;
        temp->m_returnEvent = m_waitResult[i].events;
		waitResult.emplace_back(temp);
	}
	return waitResult;
}

void EpollPoller::loop()
{
    m_enable = true;
    std::vector<Channel*> waitResult;
    while (m_enable)
    {
        waitResult = this->wait();
        for (auto &it : waitResult)
        {
            it->handle();
        }
    }
}

void EpollPoller::closeLoop()
{
    m_enable = false;
}
////////////////////////////////////////////////////////////////////
Reactor::Reactor(int port) : Reactor(port, std::thread::hardware_concurrency()) 
{
}

Reactor::Reactor(int port, int threadCount) 
: m_threadPool(std::make_unique<ThreadPool>(threadCount)), m_acceptChannel(port),
m_slaves(new EpollPoller[threadCount]), m_master(std::make_unique<EpollPoller>())
{
    for (int i=0; i<threadCount; i++)
    {
        m_threadPool->add(&EpollPoller::loop, &m_slaves[i]);
    }
    setParse(&Reactor::defaultParse, this);
    m_acceptChannel.m_reacotr = this;
}

Reactor::~Reactor() {}

void Reactor::loop()
{
    m_master->addChannel(&m_acceptChannel);
    m_master->loop();
}

std::string Reactor::defaultParse(std::string &readBuff)
{
    return readBuff;
}
////////////////////////////////////////////////////////////////////
}