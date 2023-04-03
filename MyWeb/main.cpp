#include "tools/_reactor.h"
#include "tools/_log.h"
#include "tools/_dbpool.h"
#include "tools/_http.h"

#include "src/upload.h"
#include "src/login.h"
#include "src/show.h"
#include "src/response.h"

#include <string>

std::string request{"POST / HTTP/1.1\r\n"
    "Host:img.mukewang.com\r\n"
    "User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64)"
    "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.106 Safari/537.36\r\n"
    "Accept:image/webp,image/*,*/*;q=0.8\r\n"
    "Referer:http://www.imooc.com/\r\n"
    "Accept-Encoding:gzip, deflate, sdch\r\n"
    "Accept-Language:zh-CN,zh;q=0.8\r\n"
    // POST方法必须要有Content-Length这个字段
    // 而且字段值必须要大于等于实际长度
    // 如果小于只能解析部分内容，并且errno设置为HPE_INVALID_METHOD
    "Content-Length: 5\r\n"
    "\r\n"
    "12345"};

std::string response{"HTTP/1.1 200 OK\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Server: demo26\r\n"
    "Connection: keep-alive\r\n"
    "Transfer-Encoding: chunked\r\n"
};

std::string rootpath = "/home/wucz/test/MyWeb/resource";

std::string text_type = "Content-Type: text/html;charset=utf-8\r\n\r\n";
std::string img_type = "Content-Type: image/jpeg\r\n\r\n";

int main()
{
    dya::Singleton<dya::DatebasePool>::instance("8.130.105.64,root,bilibili2022,test0715,3306", "utf8", 10, 10);
    LOG_INIT("/home/wucz/test/MyWeb/log/main", true);
    LOG_SET_LEVEL(dya::Logger::INFO);

    dya::Reactor master(5099);
    /*
    auto fn = [](std::string &readBuff)->std::string{
        LOG_INFO("\n%s\n", readBuff.c_str());
        dya::Http http;
        if (http.parser(readBuff) == false)
        {
            return response+http.m_errno;
        }
        for (auto &it : http.m_result)
        {
            LOG_DEBUG("key:%s  value:%s\n", it.first.c_str(), it.second.c_str());
        }
        // 判断是不是GET
        if (http.m_result.find("body") == http.m_result.end())
        {
            dya::Get get;
            std::string name;
            if (http.m_result["url"] == "/")
            {
                name = rootpath + "/" + "index.html";
                if (get.set(name.c_str()) == false)
                {
                    return response + "没有文件";
                }
                LOG_INFO("%s\n", "index send");
                return response + get.get();
            }
            name = rootpath + http.m_result["url"];
            if (get.set(name.c_str()) == false)
            {
                return response + "没有文件";
            }
            LOG_INFO("%s\n", "js or css send");
            return response + get.get();
        }
        return response + "不是GET";
    };

    master.setParse(fn);
    master.loop();
    */
}