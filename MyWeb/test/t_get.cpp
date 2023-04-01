#include "../src/response.h"
#include <iostream>
#include <cassert>

int main()
{
    dya::Get gg(R"(/home/wucz/test/MyWeb/test/t_get.cpp)");
    std::string ret = gg.getResponse();
    std::cout << ret << "\n";
}