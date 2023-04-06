#include "message.h"
#include <unistd.h>
#include <sstream>
#include "../tools/_dbpool.h"
#include "../tools/_log.h"
#include "../tools/nlohmann/json.hpp"
#include "../tools/_time.h"

namespace dya
{
////////////////////////////////////////////////////////////////////
std::string MessageBoard::write(const std::string &buff)
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
    // 准备插入语句
    stmt.prepare("insert into message(time, message) values(:1, :2)");
    // 绑定输入
    auto t = Time::to_string(time(0), "%Y-%m-%d %H:%M");
    stmt.bindin(1, (char*)t.c_str(), t.size());
    stmt.bindin(2, (char*)buff.c_str(), buff.size());
    // 执行
    if (stmt.execute() != 0)
    {
        LOG_DEBUG("SQL execute error! message:%s\n", stmt.m_cda.message);
        // 归还数据库连接
        dbpoolPtr->freeConnect(conn);
        return {};
    }
    // 提交事务
    conn->commit();
    // 归还数据库连接
    dbpoolPtr->freeConnect(conn);

    nlohmann::json nj;
    nj["0"] = {{"time", t.c_str()}, {"message", buff.c_str()}};
    std::stringstream ss;
    ss << nj;
    return ss.str();
}

std::string MessageBoard::read()
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
    stmt.prepare("select id,time,message from message");
    // 绑定结果集
    int id=0;
    char time[17]{0};
    char message[81]{0};
    stmt.bindout(1, &id);
    stmt.bindout(2, time, 17);
    stmt.bindout(3, message, 81);
    // 执行查询 
    if (stmt.execute() != 0)
    {
        LOG_DEBUG("SQL execute error! message:%s\n", stmt.m_cda.message);
        // 归还数据库连接
        dbpoolPtr->freeConnect(conn);
        return {};
    }

    nlohmann::json nj;
    while (true)
    {
        if (stmt.next() != 0)
            break;
        // 拼接成json
        nj[std::to_string(id)] = {{"time", time}, {"message", message}};
        id = 0;
        memset(time, 0, sizeof(time));
        memset(message, 0, sizeof(message));
    }
    // 归还数据库连接
    dbpoolPtr->freeConnect(conn);

    std::stringstream ss;
    ss << nj;
    return ss.str();
}
////////////////////////////////////////////////////////////////////
}