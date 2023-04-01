#pragma once   
#include <functional>
#include <memory>
#include <string>
#include "_network.h"

namespace dya
{
////////////////////////////////////////////////////////////////////
class Channel
{
public :
    Channel(int fd);
    Channel();
    ~Channel();
    virtual void handle();
    int getFd();
public :
    // 该socke关心的事件，一个socket可以关心多个事件
    int m_eventList;    
    // 多个事件中，最终唤醒epoll_wait的那个事件
    int m_returnEvent;
protected :
    Socket m_socket;
};
////////////////////////////////////////////////////////////////////
class Reactor;
class AcceptChannel : public Channel
{
    friend Reactor;
public :
    AcceptChannel(int port);
    ~AcceptChannel();
    void handle() override;
    void setHandle(const std::function<std::string(std::string&)>&);
private :
    int hash(int key);
private :
    Reactor *m_reacotr;
    std::function<std::string(std::string&)> m_handle;
};
////////////////////////////////////////////////////////////////////
class ClientChannel : public Channel
{
public :
    ClientChannel(int fd);
    ~ClientChannel();
    void handle() override;
    void setHandle(std::function<std::string(std::string&)>&);
    void setSelf(ClientChannel*);
private :
    void getReadBuff();
private :
    ClientChannel *m_self;
    std::string m_readBuff;
    std::string m_sendBuff;
    std::function<std::string(std::string&)> m_handle;
};
////////////////////////////////////////////////////////////////////
}