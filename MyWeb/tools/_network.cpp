#include "_network.h"
#include <cassert>
#include <fcntl.h>
#include <unistd.h> //close()在这个头文件
#include <string.h> //memset()在这个头文件
#include <netdb.h> //gethostbyname()在这个头文件

namespace dya
{
////////////////////////////////////////////////////////////////////
Socket::Socket() 
{ 
	m_fd = socket(AF_INET, SOCK_STREAM, 0); 
	assert(m_fd != -1);
}

Socket::Socket(int fd) 
{ 
	m_fd = fd;
}

Socket::~Socket() 
{
	//析构不要关闭fd
	//close(m_fd);
}

int Socket::getfd() 
{ 
	return m_fd;
}

bool Socket::bind(InetAddress &inetAddr) 
{
	//bind成功返回0，失败返回-1
	int result = ::bind(m_fd, (sockaddr *)&inetAddr.m_addr, inetAddr.m_len);
	return result+1;
}

bool Socket::listen()
{
	//listen成功返回0，失败返回-1
	int result = ::listen(m_fd, SOMAXCONN);
	return result+1;
}

bool Socket::connect(InetAddress &inetAddr)
{
	assert(m_fd != -1);
	//connect成功返回0，失败返回-1
	int result = ::connect(m_fd, (sockaddr *)&inetAddr.m_addr, inetAddr.m_len);
	return result+1;
}

int Socket::accept(InetAddress &inetaddr)
{
	return ::accept(m_fd, (sockaddr *)&inetaddr.m_addr, &inetaddr.m_len);
}

void Socket::setNonBlock()
{
	fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL) | O_NONBLOCK);
}
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//给accept当参数
InetAddress::InetAddress(): m_len(sizeof(struct sockaddr_in))
{
	memset(&m_addr, 0, sizeof(m_addr));
}
//给connect当参数
InetAddress::InetAddress(const char *ip, int port):InetAddress(port)
{
	struct hostent *h;
	h = gethostbyname(ip);
	memcpy(&m_addr.sin_addr, h->h_addr, h->h_length);
	m_len = sizeof(m_addr);
}
//给bind当参数
InetAddress::InetAddress(int port)
{
	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	//指定端口
	m_addr.sin_port = htons(port);
	//指定ip地址
	//m_addr.sin_addr.s_addr = inet_addr(ip);  //有问题
	m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_len = sizeof(m_addr);
}
InetAddress::~InetAddress(){}
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
Epoll::Epoll() 
{
	m_epfd = epoll_create(1);
	assert(m_epfd != -1);
	m_waitResult = new struct epoll_event[EVMAX];
	memset(m_waitResult, 0, sizeof(struct epoll_event)*EVMAX);
}

Epoll::~Epoll() 
{
	if (m_epfd != -1)
	{
		close(m_epfd);
		m_epfd = -1;
	}
	delete [] m_waitResult;
}

void Epoll::addFd(int fd, int op)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = op;
	ev.data.fd = fd;
	epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev);
}

const std::vector<struct epoll_event*> Epoll::wait(int timeout)
{
	int count = epoll_wait(m_epfd, m_waitResult, EVMAX, timeout);
	if(count == -1)
		return {};

	std::vector<struct epoll_event*> waitResult;
	for(int i=0; i<count; i++)
	{
		waitResult.emplace_back(&m_waitResult[i]);
	}
	return waitResult;
}
////////////////////////////////////////////////////////////////////
}
