#include "../src/show.h"
#include <iostream>

int main()
{
    //dya::ShowList sl("/home/wucz/test/MyWeb/resource");
    //std::string ret = sl.show();
    auto ret = dya::ShowFileList()("/home/wucz/test/MyWeb/resource/t1.html");
    std::cout << ret << "\n";
}