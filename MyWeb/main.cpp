#include "tools/_reactor.h"
#include "tools/_log.h"
#include "tools/_dbpool.h"
#include "tools/_http.h"

#include "src/upload.h"
#include "src/login.h"
#include "src/show.h"
#include "src/message.h"
#include "src/response.h"

#include <signal.h>
#include <string>

std::string response{"HTTP/1.1 404 Not Found\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Server: dx191dya.com\r\n"
    "Connection: keep-alive\r\n"
    "Content-Type:text/html;charset=utf-8\r\n"
    "Content-Length:61\r\n\r\n"
    "<html><head>404</head><body><h1>Not Found</h1></body></html>"
};

void EXIT(int sig)
{
    exit(0);
}

int main()
{
    // 信号处理
    for (int i=1; i<=64; ++i)
    {
        ::signal(i, SIG_IGN);
    }
    ::signal(2, EXIT);
    ::signal(15, EXIT);

    dya::Singleton<dya::DatebasePool>::instance("8.130.105.64,root,bilibili2022,test0715,3306", "utf8", 10, 10);
    LOG_INIT("/home/wucz/test/MyWeb/log/main", true);
    LOG_SET_LEVEL(dya::Logger::INFO);
    // 指定端口
    dya::Reactor master(80);

    auto fn = [](std::string &readBuff)->std::string
    {
        dya::Http http;
        http.parser(readBuff);

        if (http.find("Cookie"))
        {
            LOG_DEBUG("Cookie : %s\n", http["Cookie"].c_str());
        }

        // GET请求
        if (http["url"].compare("/post") != 0)
        {
            LOG_DEBUG("GET %s\n", http["url"].c_str());
            dya::Get get(http["url"]);
            return get.getResponse();;
        }
        else
        {
            if (http.find("Cookie") == false)
            {
                if (http["body"].compare(2, 2, "--") == 0)
                {
                    dya::Post post("No Cookie");
                    return post.getResponse();
                }
            }
            // POST请求
            LOG_DEBUG("\nPOST %s\n%s\n", http["url"].c_str(), http["body"].c_str());
            dya::Post post(http["body"]);
            return post.getResponse();
        }
        return response;
    };

    master.setParse(fn);
    master.loop();
}