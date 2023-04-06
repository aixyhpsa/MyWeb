#include "response.h"
#include <unordered_map>

#include "../tools/_directory.h"
#include "../tools/_log.h"
#include "../tools/nlohmann/json.hpp"

#include "login.h"
#include "upload.h"
#include "show.h"
#include "message.h"

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
constexpr char kHead_503[] = "HTTP/1.1 503 Service Unavailable\r\n";
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
    // 第一次访问
    if (m_name == "/")
        m_name.append("index.html");

    // 检查路径
    if (kRootPath[kRootPath.size()-1] != '/' && m_name[0] != '/')
        m_name = kRootPath + "/" + m_name;
    else
        m_name = kRootPath + m_name;
    
    std::string ret;
    File f(m_name);
    m_fin.open(m_name, std::ios::binary);

    // 文件不存在或者打开失败
    if (f.isExist() == false || m_fin.is_open() == false)
    {
        ret.append(kHead_404);
        ret.append(kAccess);
        ret.append(kContent_Length);
        ret.append("9\r\n");
        ret.append(kContent_Type);
        ret.append("text/plain");
        ret.append(kContent_Type_charset);
        ret.append("\r\n");
        ret.append("Not Found");
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
    ret.resize((f.size()+beg));
    // 从文件中读取结果
    int count = m_fin.readsome(&ret[beg], f.size());
    return ret;
}

std::string Get::getFileType()
{
    // 没有后缀名的统统当成普通文本文件处理
    int beg = m_name.find_last_of('.');
    if (beg == -1)
        return kFileType["plain"];

    std::string type = m_name.substr(beg+1);
    // 有后缀名但是这个后缀名是自定义的，也统统当成文本文件处理
    //if (kFileType[type] == "")
    if (kFileType.find(type) == kFileType.end())
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
        if (dya::Upload()(kUploadPath, m_buff))
            return this->success("上传成功");
        return this->error();
    }

    nlohmann::json jRet = nlohmann::json::parse(m_buff.c_str());
    // 登录：{"accountNumber":"", "password":""}
    if (jRet.find("accountNumber") != jRet.end())
    {
        // 防SQL注入
        dya::Login login("select count(password) from user where accountNumber=:1 and password=:2");
        if (login(jRet["accountNumber"], jRet["password"]))
            return this->success("登录成功");
        return this->error();
    }

    // 展示文件夹
    // {"showList":"rootPat"}
    if (jRet.find("showList") != jRet.end())
    {
        if (jRet["showList"] == "rootPath")
            return this->success(ShowFileList()(kUploadPath));
        std::string name;
        // 添加本地存放的途径
        name.append(kUploadPath);
        if (name[name.size()-1] != '/')
            name.append("/");
        name.append(jRet["showList"]);
        return this->success(ShowFileList()(name));
    }

    // 添加留言：{"message":"XXX"}
    if (jRet.find("message_add") != jRet.end())
    {
        MessageBoard mb;
        auto ret =  mb.write(jRet["message_add"]);
        if (ret.size() == 0)
            return this->error();
        return this->success(ret);
    }
    // 展示留言
    if (jRet.find("message_show") != jRet.end())
    {
        MessageBoard mb;
        auto ret =  mb.read();
        if (ret.size() == 0)
            return this->error();
        return this->success(ret);
    }
    // error修改
    return this->error();
}

std::string Post::success(const std::string &responseBody)
{
    std::string ret;
    ret.append(kHead_200);
    ret.append(kAccess);
    ret.append(kServer);
    ret.append(kConnection);
    ret.append(kContent_Type);
    ret.append("text/plain");
    ret.append(kContent_Type_charset);
    ret.append(kContent_Length);
    ret.append(std::to_string(responseBody.size()));
    ret.append("\r\n\r\n");
    ret.append(responseBody);
    return ret;
}

std::string Post::error()
{
    std::string ret;
    ret.append(kHead_503);
    ret.append(kAccess);
    ret.append(kContent_Length);
    ret.append("5\r\n");
    ret.append(kContent_Type);
    ret.append("text/plain");
    ret.append(kContent_Type_charset);
    ret.append("\r\n");
    ret.append("error");
    return ret;
}
////////////////////////////////////////////////////////////////////
} 