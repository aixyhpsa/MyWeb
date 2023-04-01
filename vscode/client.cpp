#include <iostream>
#include "../newTools/_network.h"
#include <cassert>
#include <string.h>
#include <unistd.h>

int main()
{
    dya::InetAddress sevInfo("8.130.105.64", 5099);
    dya::Socket mySock;
    assert(mySock.connect(sevInfo));
    char buff[128]{0};
    strcpy(buff, "1234321");
    send(mySock.getfd(), buff, strlen(buff), 0);

    //sleep(20);
    memset(buff, 0, sizeof(buff));
    recv(mySock.getfd(), buff, sizeof(buff), 0);
    std::cout << buff << "\n";
}