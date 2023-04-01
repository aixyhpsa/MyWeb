#include "upload.h"
#include <iostream>

int main()
{
    dya::ShowList sl("/home/wucz/test/MyWeb/resource");
    std::string ret = sl.show();
    std::cout << ret << "\n";
}