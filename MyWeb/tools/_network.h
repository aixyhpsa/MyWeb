#pragma once
#include <sys/types.h> //含有size_t，time_t，pid_t等类型
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <vector>

#define EVMAX 20
namespace dya
{
////////////////////////////////////////////////////////////////////
class InetAddress
{
public :
	//给accept当参数
	InetAddress();

	//给connect当参数
	InetAddress(const char *ip, int port);

	//给bind当参数
	InetAddress(int port);

	~InetAddress();
public :
	struct sockaddr_in m_addr;
	socklen_t m_len;
};
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
class Socket
{
public :
	Socket();
	Socket(int fd);
	~Socket();
public :
	/**
	 * @brief 与::bind效果一样，服务端使用
	 * param [in] 
	 * @return 成功为true；失败为false
	 */
	bool bind(InetAddress &inetAddr);
	//服务端使用2
	bool listen();
	//服务端使用3
	int accept(InetAddress &inetAddr);

	//客户端使用
	bool connect(InetAddress &inetAddr);

	//返回fd
	int getfd();

	//将socket设置成非阻塞
	void setNonBlock();
private :
	int m_fd;
};
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
class Epoll
{
public :
	Epoll();
	~Epoll();
public :
	//LT(水平触发);ET(边沿触发)
	void addFd(int fd, int op = EPOLLIN);

    const std::vector<struct epoll_event*> wait(int timeout = -1);
private :
	int m_epfd;
	struct epoll_event *m_waitResult;
};
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
/*服务端示例：
#include <iostream>
#include "/home/wucz/test/tools/_socket.h"

int main()
{
    dya::InetAddress sevInfo(5099);
    dya::Socket sev;

    if (!sev.bind(sevInfo))
        std::cout << "bind error\n";
    if (!sev.listen())
        std::cout << "listen error\n";

    while(true)
    {
        dya::InetAddress acpInfo;
        int acpFd = sev.accept(acpInfo);
        //std::cout << "sin_port:"<< acpInfo.m_addr.sin_port;
        //std::cout << " sin_addr:"<< acpInfo.m_addr.sin_addr.s_addr << "\n";
        char buff[1024]{0};
        int sum = recv(acpFd, buff, 1024, 0);
        if (sum <= 0)
            break;
        std::cout << buff << "\n";
    }
    return 0;
}
*/
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
/*客户端示例：
#include <iostream>
#include "/home/wucz/test/tools/_socket.h"

int main()
{
    dya::InetAddress sevInfo("8.130.105.64", 5099);
    dya::Socket cli;
    if (!cli.connect(sevInfo))
        std::cout << "error\n";
    char buff[128]{0};
    strcpy(buff, "hello");
    send(cli.getfd(), buff, strlen(buff), 0);
    return 0;
}
*/
////////////////////////////////////////////////////////////////////
}