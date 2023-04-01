#include "response.h"
#include <unordered_map>
#include "../tools/_directory.h"
#include "nlohmann/json.hpp"
#include "login.h"

namespace dya
{
////////////////////////////////////////////////////////////////////
template<class T1, class T2>
using umap = std::unordered_map<T1, T2>;

umap<std::string, std::string> kFileType{
{"html", "text/html"}, {"css", "text/css"}, {"js", "text/javascript"}, {"plain", "text/plain"},
{"jpg", "image/jpeg"}, {"png", "image/png"}, {"bmp", "image/bmp"}, 
{"webp", "image/bmp"}, {"gif", "image/gif"}, {"ico", "image/icon"},
{"mp3", "audio/mpeg"}, {"midi", "audio/midi"}, {"mp4", "video/mp4"}, {"webm", "video/webm"}, {"ogg", "video/ogg"},
{"gz", "application/x-gzip"}, {"tar", "application/x-tar"}, {"zip", "application/zip"}, {"rar", "application/rar"},
{"rtf", "application/rtf"}, {"pdf", "application/pdf"}, {"xml", "application/xml"}, {"json", "application/json"},
{"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"}, 
{"pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"}, 
{"xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"}, 
{"exe", "application/octet-stream"}
};
////////////////////////////////////////////////////////////////////
constexpr char kHead_200[] = "HTTP/1.1 200 OK\r\n";
constexpr char kHead_404[] = "HTTP/1.1 404 Not Found\r\n";
constexpr char kServer[] = "Server: dx191dya.com\r\n";
constexpr char kAccess[] = "Access-Control-Allow-Origin: *\r\n";
constexpr char kConnection[] = "Connection: keep-alive\r\n";
constexpr char kContent_Length[] = "Content-Length:";
constexpr char kContent_Type_charset[] = ";charset=utf-8\r\n";
constexpr char kContent_Type[] = "Content-Type:";
////////////////////////////////////////////////////////////////////
Get::Get(std::string &name) : m_name(name) {}

Get::Get(std::string &&name) : m_name(std::move(name)) {}

std::string Get::getResponse()
{
    std::string ret;
    File f(m_name);
    m_fin.open(m_name, std::ios::binary);

    if (f.isExist() == false || m_fin.is_open() == false)
    {
        ret.append(kHead_404);
        ret.append(kAccess);
        ret.append(kContent_Length);
        ret.append("61\r\n");
        ret.append(kContent_Type);
        ret.append("text/html");
        ret.append(kContent_Type_charset);
        ret.append("\r\n");
        ret.append("<html><head>404</head><body><h1>Not Found</h1></body></html>");
        return ret;
    }

    ret.append(kHead_200);
    ret.append(kServer);
    ret.append(kAccess);
    ret.append(kConnection);
    ret.append(kContent_Length);
    ret.append(std::to_string(f.size()));
    ret.append("\r\n");
    ret.append(kContent_Type);
    // 确定文件类型
    ret.append(getFileType());
    ret.append(kContent_Type_charset);
    ret.append("\r\n");

    int beg = ret.size();
    ret.resize(f.size()+beg);
    m_fin.read(&ret[beg], f.size());
    return ret;
}

std::string Get::getFileType()
{
    int beg = m_name.find_last_of('.');
    if (beg == -1)
        return kFileType["plain"];

    std::string type = m_name.substr(beg+1);
    if (kFileType[type] == "")
        return kFileType["plain"];
    return kFileType[type];
}
////////////////////////////////////////////////////////////////////
Post::Post(const std::string &buff) :m_buff(buff) {}

std::string Post::getResponse()
{
    // 登录？上传文件？添加留言？
    // 上传
    if (m_buff.compare(2, 2, "--") == 0)
    {
        return {};
    }

    nlohmann::json jRet = nlohmann::json::parse(m_buff.c_str());

    // 登录：{"accountNumber":"root", "password":"368751429QQ"}
    if (jRet["accountNumber"] != "")
    {
        return {};
    }

    // 添加留言：{"messageBoard":"XXX"}
    if (jRet["messageBoard"] != "")
    {

        return {};
    }

    // 展示文件夹

    // 展示留言
    return {};
}

std::string Post::upload()
{
    // 从buff中找到文件名称
    int beg = m_buff.find("filename=");
    if (beg == -1)
        return this->error();
    beg = beg + sizeof(R"(filename=")");
    int end = m_buff.find(R"(")", beg);
    std::string fileName = m_buff.substr(beg, end-beg);
}
////////////////////////////////////////////////////////////////////
} 