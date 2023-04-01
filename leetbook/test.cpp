#include <iostream>
#include <fstream>
#include <signal.h>
#include <unistd.h>


int main()
{
    system("ftp -n 8.130.105.64 < ../input.txt > ../out.txt");
    //system("ftp -n 8.130.105.64 user wucz eEhgxQdz-w9w8vY");
    //system("ls >> ./ls.log");
    return 0;
}