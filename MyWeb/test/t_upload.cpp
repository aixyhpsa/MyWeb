#include "../src/upload.h"
#include <iostream>

int main()
{
    std::string body{
        R"(------WebKitFormBoundaryiMI1QvCnWpIffAMF)""\r\n"
        R"(Content-Disposition: form-data; name="upload"; filename="123.html")""\r\n"
        R"(Content-Type: text/plain)""\r\n"
        R"(0-4    二本+锅+大理石甲+草根+火腿棒+猪皮头+帐篷)""\n"
        R"(5-7    骑牛探路+蓝蘑菇林拿活木)""\n"
        R"(8-11   星杖+魔光+唤灵)""\n"
        R"(12-14  准备龙蝇物资)""\n"
        R"(15     龙蝇)""\n"
        "\r\n"
        R"(------WebKitFormBoundaryiMI1QvCnWpIffAMF--)"
    };

    std::cout << "ret:" << dya::Upload()("/home/wucz/test/MyWeb/resource/upload", body) << "\n";
}