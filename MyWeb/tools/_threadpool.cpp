#include "_threadpool.h"
#include <thread>
#include <chrono>

namespace dya
{
//默认构造，如果不指定线程数，则根据硬件设置线程数
ThreadPool::ThreadPool() : ThreadPool(std::thread::hardware_concurrency())
{
}

ThreadPool::ThreadPool(int count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        //消费者函数，使用lambda表达式的形式表示，不单独写成成员函数
        std::thread([this]{
            while (m_EndFlag)
            {
                std::unique_lock<std::mutex> ulock(m_mutex);
                if (!m_queue.empty())
                {
                    auto task = std::move(m_queue.front());
                    m_queue.pop();
                    ulock.unlock();
                    task();
                    ulock.lock();
                }
                else
                {
                    m_cond.wait(ulock);
                }
            }
            //分离线程
            }).detach();
    }
}

ThreadPool::~ThreadPool()
{
    m_EndFlag = false;
    m_cond.notify_all();
    //等待100毫秒，等待全部线程退出
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
}