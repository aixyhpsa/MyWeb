#include "_channel.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <cassert>
#include "_reactor.h"

namespace dya
{

////////////////////////////////////////////////////////////////////
Channel::Channel(int fd) : m_socket(fd)
{
    m_eventList = 0;
    m_returnEvent = 0;
}

Channel::Channel()
{
    m_eventList = 0;
    m_returnEvent = 0;
}

Channel::~Channel() {}

int Channel::getFd()
{
    return m_socket.getfd();
}

void Channel::handle() {}
////////////////////////////////////////////////////////////////////
AcceptChannel::AcceptChannel(int port)
{
    InetAddress serverInfo(port);
    assert(m_socket.bind(serverInfo));
    assert(m_socket.listen());
    //m_socket.setNonBlock();
    // EPOLLIN比较万能，有新连接、有收到数据、客户端断开连接都会触发此事件
    m_eventList = EPOLLIN | EPOLLET;
}

AcceptChannel::~AcceptChannel() {}

void AcceptChannel::handle()
{
    InetAddress temp;
    Socket acceptSock(m_socket.accept(temp));
    // 把acceptSock设置为非阻塞，此步骤容易忘记
    acceptSock.setNonBlock();

    ClientChannel * ch = new ClientChannel(acceptSock.getfd());
    ch->setHandle(m_handle);
    ch->setSelf(ch);
    int number = hash(acceptSock.getfd());
    m_reacotr->m_slaves[number].addChannel(ch);
}

void AcceptChannel::setHandle(const std::function<std::string(std::string&)>& parse)
{
    m_handle = parse;
}

int AcceptChannel::hash(int key)
{
    //return key%(m_reacotr->m_slaves.size());
    return key%2;
}
////////////////////////////////////////////////////////////////////
ClientChannel::ClientChannel(int fd) : Channel(fd) {}

ClientChannel::~ClientChannel() {}

void ClientChannel::handle()
{
    this->getReadBuff();
    // 客户端正常断开连接
    if (m_readBuff.size() == 0)
    {
        // 关闭fd，epoll会自动删除节点
        close(m_socket.getfd());
        // 自己删除自己
        delete m_self;
        return;
    }
    // 如果你没有使用setSendBuff()去设置个性化回调函数，
    // 那么这里将使用框架自带的最简单的回调函数，即defaultSetSendBuff()，功能为echo
    if (m_handle != nullptr)
        m_sendBuff = this->m_handle(m_readBuff);

    int count = 0;
    int sum = 0;
    //std::cout << "一共要发送:" << m_sendBuff.size() << "\n";
    while (true)
    {
        count = send(m_socket.getfd(), &m_sendBuff[sum], m_sendBuff.size()-sum, 0);
        //std::cout << "这次发送了:" << count << "\n";
        // 出现 EAGAIN 一般是发送缓冲区满了，要稍等一会再发的意思
        if (count == -1 && errno == EAGAIN)
        {
            usleep(10);
            continue;
        }
        if (count == -1 && errno == EINTR)
            continue;
        if (count == 0)
            break;
        sum += count;
        if (sum == m_sendBuff.size())
            break;
    }
    //std::cout << "这次发送完毕了\n\n";
    m_readBuff.clear();
    m_sendBuff.clear();
}

void ClientChannel::getReadBuff() 
{
    char tempBuff[1024]{0};
    while(true)
    {
        // -1代表错误；0代表断开连接
        int bytes = recv(m_socket.getfd(), tempBuff, 1024, 0);
        // 客户端正常中断，继续读取。
        // EINTR  4  中断的系统调用(interrupted system call)
        if (bytes == -1 && (errno == EINTR))
            continue;
        // 非阻塞IO，这个条件表示数据全部读取完毕
        // EAGIN  11  再试一次(try again)
        // EWOULDBLOCK  11  操作将阻塞(operation would block)
        if (bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
            break;
        // 断开连接
        if (bytes == 0)
        {
            m_readBuff.clear();
            break;
        }
        // 正常读取
        m_readBuff.assign(tempBuff, 0, bytes);
        memset(tempBuff, 0, bytes);
    }
}

void ClientChannel::setHandle(std::function<std::string(std::string&)>& parse)
{
    m_handle = parse;
}

void ClientChannel::setSelf(ClientChannel *ch)
{
    m_self = ch;
}
////////////////////////////////////////////////////////////////////
}