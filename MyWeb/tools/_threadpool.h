#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <memory>
#include <type_traits>
#include <future>

namespace dya
{
class ThreadPool
{
private:
    std::mutex m_mutex;
    std::condition_variable m_cond;
    bool m_EndFlag = true;
    std::queue<std::function<void()>> m_queue;
public :
    ThreadPool(int);
    //默认构造，如果不指定线程数，则根据硬件设置线程数
    ThreadPool();
    ~ThreadPool();
public :
    template<typename F, class... Args>
    auto add(F &&fn, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type> 
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(fn), std::forward<Args>(args)...)
            );

        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> ulock(m_mutex);
            m_queue.emplace([task]{ (*task)(); });
        }

        m_cond.notify_one();
        return res;
    }
};

}