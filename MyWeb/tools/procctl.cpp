#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc,char *argv[])
{
    if (argc<3)
    {
        printf("Using:./procctl timetvl program argv ...\n");
        printf("Example:/project/tools1/bin/procctl 5 /usr/bin/tar zcvf /tmp/tmp.tgz /usr/include\n\n");

        printf("本程序是服务程序的调度程序，周期性启动服务程序或shell脚本。\n");
        printf("timetvl 运行周期，单位：秒。被调度的程序运行结束后，在timetvl秒后会被procctl重新启动。\n");
        printf("program 被调度的程序名，必须使用全路径。\n");
        printf("argvs   被调度的程序的参数。\n");
        printf("注意，本程序不会被kill杀死，但可以用kill -9强行杀死。\n\n\n");

        return -1;
    }
    // kill(pid,sig)
    // sig：准备发送的信号代码，假如其值为零则没有任何信号送出，但是系统会执行错误检查，通常会利用sig值为零来检验某个进程是否仍在运行。
    // 返回值说明： 成功执行时，返回0；失败返回-1，errno被设为以下的某个值。
    // EINVAL：指定的信号码无效（参数 sig 不合法）。
    // EPERM：权限不够无法传送信号给指定进程。
    // ESRCH：参数 pid 所指定的进程或进程组不存在。
    // 本程序使用kill(pid, 0)来判断程序是否存活

    // 忽略子进程的信号，如果不处理信号并且不使用wait函数，那么子进程结束后会变成僵尸进程，僵尸进程会干扰kill(pid, 0)的结果
    //signal(SIGCHLD,SIG_IGN);

    // 关闭信号和IO，本程序不希望被打扰。
    for (int ii=0;ii<64;ii++)
    {
        signal(ii,SIG_IGN);
        close(ii);
    }

    // 让程序运行在后台，由系统1号进程托管。
    if (fork()!=0) exit(0);

    char *pargv[argc];
    for (int ii=2;ii<argc;ii++)
    {
        pargv[ii-2]=argv[ii];
    }
    pargv[argc-2]=NULL;

    while (true)
    {
        int pid = fork();
        if (pid == 0)
        {
            execv(argv[2],pargv);
            exit(0);
        }
        else
        {
            while (true)
            {
                if (kill(pid, 0) != 0)
                    break;
                sleep(atoi(argv[1]));
            }
            //printf("子进程已经结束\n");
        }
    }

}