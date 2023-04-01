#include <iostream>
#include <fstream>
#include <signal.h>
#include <unistd.h>

std::fstream fp;
int targetPid = 0;
void handler(int id)
{
    std::cout << "写入前=" << fp.tellp() << " ";
    fp.seekg(0, std::ios::end);
    std::cout << "移东="<< fp.tellp() << "\n";
    fp << getpid() << "\n";
    fp.flush();
    sleep(1);
    kill(targetPid, 10);
}

int main()
{
    std::cout << "pid= " << getpid() << "\n" << "请输入目标id:";
    fp.open("../ls.log");
    std::cin >> targetPid;

    signal(10, handler);
    sleep(4);

    kill(targetPid, 10);
    for(int i=0; i<10; i++)
    {
        sleep(30);
    }
    fp.close();
    //system("ftp -n 8.130.105.64 user wucz eEhgxQdz-w9w8vY");
    //system("ls >> ./ls.log");
    return 0;
}