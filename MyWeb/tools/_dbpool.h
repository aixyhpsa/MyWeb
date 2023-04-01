#pragma once   
#include <mutex>
#include <string>
#include <memory>
#include "_mysql.h"
#include "_singleton.hpp"

namespace dya
{
////////////////////////////////////////////////////////////////////
class DatebasePool
{
public :
    friend Singleton<DatebasePool>;
    struct st_conn 
    {
        connection m_conn;
        std::mutex m_mutex;
        int m_atime = 0;
    };
    
    connection* getConnect();
    bool freeConnect(connection*);
    void checkTimeout();
    void destroy();
private :
    // 登录信息、字符集、最大连接数、超时时间
    DatebasePool(const char*, const char*, int, int);
    ~DatebasePool();
    int getUTCTime();   
private :
    std::unique_ptr<st_conn[]> m_pool;
    std::string m_loginInfo;
    std::string m_charset;
    int m_timeout = 0;
    int m_maxConnect = 0;
};
////////////////////////////////////////////////////////////////////
}