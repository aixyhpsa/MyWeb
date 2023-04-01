#include <iostream>   
#include "login.h"
#include "../tools/_dbpool.h"
#include "../tools/_log.h"

using DatebasePool = dya::Singleton<dya::DatebasePool>;

int main()
{
    DatebasePool::instance("8.130.105.64,root,bilibili2022,test0715,3306", "utf8", 20, 10);
    LOG_INIT("/home/wucz/test/MyWeb/log/t_login", true);
    std::string param = "accountNumber=root&password=368751429QQ";
    dya::Login login("select count(password) from user where accountNumber=\"%s\" and password=\"%s\";");
    bool ret = login(param.c_str());
    std::cout << "ret=" << ret << "\n";
}