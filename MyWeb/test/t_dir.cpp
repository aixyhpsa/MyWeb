#include "_directory.h"
#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main()
{
    dya::Dir dir("/home/wucz/test/MyWeb/resource");
    dir.readdir();
    //for (auto &it : dir.m_ret)
    //{
      //  std::cout << "name:" << it->d_name << "\n";
        //}
    /* 
    struct stat buff;
    int ret = stat("/home/wucz/test/MyWeb/resource", &buff);
    std::cout << "ret=" << ret << "\n";
    std::cout << strerror(errno) << "\n";
    dya::File f1("/home/wucz/test/MyWeb/resource");
    std::cout << "is_exist:" << f1.isExist() << "\n";
    */
    dya::File f1("/home/wucz/test/MyWeb/test/t_dir.cpp");
    std::cout << "mtime:" << f1.mtime() << "\n";
    std::cout << "atime:" << f1.atime() << "\n";
    std::cout << "time=" << time(0) << "\n";
}