#pragma once   
#include <mutex>
#include <string>
#include <memory>
#include <thread>
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
    std::unique_ptr<st_conn[]> m_pool;      // 数据库连接数组
    std::string m_loginInfo;                // 数据库登录信息
    std::string m_charset;                  // 数据库字符集
    int m_timeout = 0;                      // 超时时间
    int m_maxConnect = 0;                   // 最大连接数
    std::thread m_check;                    // 检查连接是否超时的线程           
    int m_isExit= false;                    // 析构时关闭上一行提到的线程的标志位，std::thread好像没有线程取消功能，所以这样操作
};
////////////////////////////////////////////////////////////////////
}