#include "_dbpool.h"
#include <cassert>
#include <chrono>

namespace dya
{
////////////////////////////////////////////////////////////////////
DatebasePool::DatebasePool(const char *loginInfo, const char *charset, int maxConnect, int timeout)
: m_timeout(timeout), m_charset(charset), m_loginInfo(loginInfo), m_maxConnect(maxConnect),
m_pool(new st_conn[maxConnect])
{
    int ret = connection().connecttodb(loginInfo, charset);
    assert(ret == 0);
}

DatebasePool::~DatebasePool()
{
    destroy();
}

connection* DatebasePool::getConnect()
{
    for (int i=0; i<m_maxConnect; ++i)
    {
        if (m_pool[i].m_mutex.try_lock())
        {
            if (m_pool[i].m_conn.m_state == false)
            {
                int ret = m_pool[i].m_conn.connecttodb(m_loginInfo.c_str(), m_charset.c_str());
                if (ret != 0)
                    return nullptr;
            }
            m_pool[i].m_atime = getUTCTime();

            return &m_pool[i].m_conn;
        }
    }
    return nullptr;
}

bool DatebasePool::freeConnect(connection *conn)
{
    for (int i=0; i<m_maxConnect; ++i)
    {
        if (conn == &m_pool[i].m_conn)
        {
            m_pool[i].m_mutex.unlock();
            m_pool[i].m_atime = getUTCTime();
            return true;
        }
    }
    return false;
}

void DatebasePool::checkTimeout()
{
    auto now = std::chrono::system_clock::now();
    auto UTCTime = std::chrono::system_clock::to_time_t(now);
    for (int i=0; i<m_maxConnect; ++i)
    {
        // 通过尝试加锁，判断该连接是否正在使用中
        if (!m_pool[i].m_mutex.try_lock())
            continue;
        // 查看是否超时
        if (UTCTime-m_pool[i].m_atime <= m_timeout)
            continue;

        m_pool[i].m_conn.disconnect();
        m_pool[i].m_atime = 0;
        m_pool[i].m_mutex.unlock();
    }
}

void DatebasePool::destroy()
{
    for (int i=0; i<m_maxConnect; ++i)
    {
        m_pool[i].m_conn.disconnect();
        m_pool[i].m_mutex.unlock();
        m_pool[i].m_atime = 0;
    }
}

int DatebasePool::getUTCTime()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(now);
}
////////////////////////////////////////////////////////////////////
}