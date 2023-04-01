#pragma once
#include <string>

namespace dya
{
////////////////////////////////////////////////////////////////////
class Login
{
public :
    /**
     * @brief 构造函数，需要输入SQL语句
     * @param 例如：select count(password) from user where accountNumber=\"%s\" and password=\"%s\";
     */
    Login(std::string &SQL);
    Login(std::string &&SQL);

    /**
     * @brief 已经弃用
     */
    bool operator()(const std::string &buff);

    /**
     * @brief 查询数据库，比较账号密码是否正确
     * @param 账号
     * @param 密码
     * @return bool
     */
    bool operator()(const std::string &, const std::string &);
private :
    std::string m_SQL;
};
////////////////////////////////////////////////////////////////////
} 
