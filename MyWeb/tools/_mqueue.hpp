#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>

namespace dya 
{
////////////////////////////////////////////////////////////////////
template<class T>
class MessageQueue
{
public :
    void push(const T&);
    T pop();
    void destroy();
private :
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    bool m_enable = true;
};
////////////////////////////////////////////////////////////////////
template<class T>
void MessageQueue<T>::push(const T &item)
{
    {
        std::unique_lock<std::mutex> ulock(m_mutex);
        m_queue.push(item);
        //m_queue.emplace(item);
    }
    m_cond.notify_one();
}

template<class T>
T MessageQueue<T>::pop()
{
    std::unique_lock<std::mutex> ulock(m_mutex);
    while (m_queue.size()==0 && m_enable == true)
    {
        m_cond.wait(ulock);
    }
    if (m_enable == false)
        return m_queue.front();
    auto item = m_queue.front();
    m_queue.pop();
    return item;
}

template<class T>
void MessageQueue<T>::destroy()
{
    m_enable = false;
    m_cond.notify_all();
}
////////////////////////////////////////////////////////////////////
}