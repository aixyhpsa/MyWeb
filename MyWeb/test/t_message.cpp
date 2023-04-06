#include "../src/message.h"
#include "../tools/_log.h"
#include "../tools/_dbpool.h"
#include <string>
#include <iostream>

using DatebasePool = dya::Singleton<dya::DatebasePool>;
int main()
{
    DatebasePool::instance("8.130.105.64,root,bilibili2022,test0715,3306", "utf8", 20, 10);
    LOG_INIT("/home/wucz/test/MyWeb/log/t_message", true);

    std::string buff{"t_message.cpp 01"};
    dya::MessageBoard mb;
    auto ret = mb.write(buff);
    std::cout << "write:" << ret << "\n";
    ret = mb.read();
    std::cout << "read:" << ret << "\n";
}