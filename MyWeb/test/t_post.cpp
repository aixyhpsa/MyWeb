#include "../src/response.h"
#include <string>
#include <iostream>
#include "../tools/_log.h"
#include "../tools/_dbpool.h"

std::string t_upload{
    R"(------WebKitFormBoundaryiMI1QvCnWpIffAMF)""\r\n"
    R"(Content-Disposition: form-data; name="upload"; filename="33.html")""\r\n"
    R"(Content-Type: text/plain)""\r\n"
    R"(0-4    二本+锅+大理石甲+草根+火腿棒+猪皮头+帐篷)""\n"
    R"(5-7    骑牛探路+蓝蘑菇林拿活木)""\n"
    R"(8-11   星杖+魔光+唤灵)""\n"
    R"(12-14  准备龙蝇物资)""\n"
    R"(15     龙蝇)""\n"
    "\r\n"
    R"(------WebKitFormBoundaryiMI1QvCnWpIffAMF--)"
};

std::string t_login_true{R"({"accountNumber":"root", "password":"368751429QQ"})"};
std::string t_login_false{R"({"accountNumber":"root", "password":"12312"})"};

std::string t_showFileList_root{R"({"showList":"rootPath"})"};
// 本地存放路径程序会自己加上
std::string t_showFileList_subdir{R"({"showList":"33.html"})"};

using DatebasePool = dya::Singleton<dya::DatebasePool>;
int main()
{
    LOG_INIT("/home/wucz/test/MyWeb/log/t_post", true);
    DatebasePool::instance("8.130.105.64,root,bilibili2022,test0715,3306", "utf8", 20, 10);

    dya::Post p1(t_upload);
    auto r1 = p1.getResponse();
    std::cout << r1 << "\n------------------\n";

    dya::Post p2(t_login_false);
    auto r2 = p2.getResponse();
    std::cout << r2 << "\n------------------\n";

    dya::Post p3(t_login_true);
    auto r3 = p3.getResponse();
    std::cout << r3 << "\n------------------\n";

    dya::Post p4(t_showFileList_root);
    auto r4 = p4.getResponse();
    std::cout << r4 << "\n------------------\n";

    dya::Post p5(t_showFileList_subdir);
    auto r5 = p5.getResponse();
    std::cout << r5 << "\n------------------\n";
}