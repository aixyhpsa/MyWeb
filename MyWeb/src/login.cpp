#include "login.h"
#include "../tools/_http.h"
#include "../tools/_dbpool.h"
#include "../tools/_log.h"

namespace dya
{
////////////////////////////////////////////////////////////////////
Login::Login(std::string &SQL): m_SQL(SQL) {}

Login::Login(std::string &&SQL): m_SQL(std::move(SQL)) {}

bool Login::operator()(const std::string &buff)
{
    // 从buff中拆出账号
    int beg = buff.find("accountNumber");
    if (beg== -1) return false;
    beg = beg + sizeof("accountNumber");
    int end = buff.find('&', beg);
    std::string accountNumber = buff.substr(beg, end-beg);
    LOG_DEBUG("账号:%s\n", accountNumber.c_str());

    // 拆出密码
    beg = buff.find("password");
    if (beg== -1) return false;
    beg = beg + sizeof("password");
    end = buff.size();
    std::string password = buff.substr(beg, end-beg);
    LOG_DEBUG("密码:%s\n", password.c_str());

    // 获取数据库连接
    auto dbpoolPtr = Singleton<DatebasePool>::getInstance();
    connection *conn = nullptr;
    while (true)
    {
        conn = dbpoolPtr->getConnect();
        if (conn != nullptr)
            break;
    }
    sqlstatement stat(conn);

    // 准备查询语句
    stat.prepare(m_SQL.c_str(), accountNumber.c_str(), password.c_str());
    LOG_DEBUG(m_SQL.c_str(), accountNumber.c_str(), password.c_str());
    // 绑定结果集
    int ret = 0;
    stat.bindout(1, &ret);
    // 执行查询语句
    if (stat.execute() != 0)
    {
        // 查询失败写一行日志
        LOG_DEBUG("execute error! message:%s\n", stat.m_cda.message);
    }
    // 取出结果并比较
    stat.next();
    if (ret == 1) 
        return true;

    // 归还数据库连接
    dbpoolPtr->freeConnect(conn);
    return false;
}


bool Login::operator()(const std::string &accountNumber, const std::string &password)
{
    // 获取数据库连接
    auto dbpoolPtr = Singleton<DatebasePool>::getInstance();
    connection *conn = nullptr;
    while (true)
    {
        conn = dbpoolPtr->getConnect();
        if (conn != nullptr)
            break;
    }
    sqlstatement stmt(conn);

    // 准备查询语句
    //stat.prepare(m_SQL.c_str(), accountNumber.c_str(), password.c_str()); // 有SQL注入的漏洞
    stmt.prepare(m_SQL.c_str());
    LOG_DEBUG("%s\n", stmt.m_sql);
    // 绑定输入参数
    stmt.bindin(1, (char*)accountNumber.c_str(), accountNumber.size());
    stmt.bindin(2, (char*)password.c_str(), password.size());
    // 绑定结果集
    int ret = 0;
    stmt.bindout(1, &ret);
    // 执行查询语句
    if (stmt.execute() != 0)
    {
        // 查询失败写一行日志
        LOG_DEBUG("SQL execute error! message:%s\n", stmt.m_cda.message);
    }
    // 取出结果并比较
    stmt.next();
    if (ret == 1) 
        return true;

    // 归还数据库连接
    dbpoolPtr->freeConnect(conn);
    return false;
}
////////////////////////////////////////////////////////////////////
}