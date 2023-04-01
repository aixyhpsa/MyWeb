#include "get.h"
#include "../tools/_directory.h"
#include "tools/nlohmann/json.hpp"
#include <unordered_map>
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
////////////////////////////////////////////////////////////////////
constexpr char kContent_Type_charset[] = ";charset=utf-8\r\n";
constexpr char kContent_Type[] = "Content-Type:";

constexpr char kContent_Type_html[] = "text/html";
constexpr char kContent_Type_css[] = "text/css";
constexpr char kContent_Type_js[] = "text/javascript";
constexpr char kContent_Type_plain[] = "text/plain";

constexpr char kContent_Type_jpeg[] = "image/jpeg";
constexpr char kContent_Type_png[] = "image/png";
constexpr char kContent_Type_bmp[] = "image/bmp";
constexpr char kContent_Type_webp[] = "image/webp";
constexpr char kContent_Type_gif[] = "image/gif";
// ico 图像格式，通常用于浏览器 Favicon 图标
constexpr char kContent_Type_icon[] = "image/icon";

constexpr char kContent_Type_mp3[] = "audio/mpeg";
constexpr char kContent_Type_midi[] = "audio/midi";

constexpr char kContent_Type_mp4[] = "video/mp4";
constexpr char kContent_Type_webm[] = "video/webm";
constexpr char kContent_Type_ogg[] = "video/ogg";

constexpr char kContent_Type_gz[] = "application/x-gzip";
constexpr char kContent_Type_tar[] = "application/x-tar";
constexpr char kContent_Type_zip[] = "application/zip";
constexpr char kContent_Type_rar[] = "application/rar";
// RTF是指Rich Text Format，即通常所说的富文本格式
constexpr char kContent_Type_rtf[] = "application/rtf";
constexpr char kContent_Type_pdf[] = "application/pdf";
constexpr char kContent_Type_xml[] = "application/xml";
constexpr char kContent_Type_json[] = "application/json";
// 微软Office Word文档格式
constexpr char kContent_Type_word[] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
// 微软Office excel文档格式
constexpr char kContent_Type_excel[] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
// 微软Office ppt文档格式
constexpr char kContent_Type_ppt[] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
// Windows系统可执行文件格式
constexpr char kContent_Type_exe[] = "application/octet-stream";
////////////////////////////////////////////////////////////////////
std::string getFileType(std::string &name)
{
    int beg = name.find_last_of('.');
    if (beg == -1)
        return kContent_Type_plain;

    std::string type = name.substr(beg+1);
    // .h .c
    if (type.size() == 1)
    {
        return kContent_Type_plain;
    }
    // .md .js
    if (type.size() == 2)
    {
        if (type == "js")
            return kContent_Type_js;
        else if (type == "md")
            return kContent_Type_plain;
        else if (type == "gz")
            return kContent_Type_gz;
        else 
            return kContent_Type_plain;
    }
    if (type.size() == 3)
    {
        if (type == "css")
            return kContent_Type_css;
        else if (type == "txt")
            return kContent_Type_plain;
        else if (type == "jpg")
            return kContent_Type_jpeg;
        else if (type == "png")
            return kContent_Type_png;
        else if (type == "bmp")
            return kContent_Type_bmp;
        else if (type == "gif")
            return kContent_Type_gif;
        else if (type == "ico")
            return kContent_Type_icon;
        else if (type == "xml")
            return kContent_Type_xml;
        else if (type == "cpp")
            return kContent_Type_plain;
        else if (type == "hpp")
            return kContent_Type_plain;
        else if (type == "rtf")
            return kContent_Type_rtf;
        else if (type == "zip")
            return kContent_Type_zip;
        else if (type == "rar")
            return kContent_Type_rar;
        else if (type == "tar")
            return kContent_Type_tar;
        else if (type == "mp3")
            return kContent_Type_mp3;
        else if (type == "mp4")
            return kContent_Type_mp4;
        else if (type == "ogg")
            return kContent_Type_ogg;
        else if (type == "pdf")
            return kContent_Type_pdf;
        else if (type == "exe")
            return kContent_Type_exe;
        else
            return kContent_Type_plain;
    }
    if (type.size() == 4)
    {
        if (type == "html")
            return kContent_Type_html;
        else if (type == "webp")
            return kContent_Type_webp;
        else if (type == "json")
            return kContent_Type_json;
        else if (type == "midi")
            return kContent_Type_midi;
        else if (type == "webm")
            return kContent_Type_webm;
        else if (type == "docx")
            return kContent_Type_word;
        else if (type == "pptx")
            return kContent_Type_ppt;
        else if (type == "xlsx")
            return kContent_Type_excel;
        else
            return kContent_Type_plain;
    }
    return kContent_Type_plain;
}
////////////////////////////////////////////////////////////////////
Get::Get(std::string &name) : m_name(name) {}

Get::Get(std::string &&name) : m_name(std::move(name)) {}

std::string Get::getResponse()
{
    File f(m_name);
    if (f.isExist() == false)
        return kHead_404;

    m_fin.open(m_name, std::ios::binary);
    if (m_fin.is_open() == false)
        return kHead_404;

    std::string ret;
    ret.append(kHead_200);
    ret.append(kServer);
    ret.append(kAccess);
    ret.append(kConnection);
    ret.append(kContent_Length);
    ret.append(std::to_string(f.size()));
    ret.append("\r\n");
    ret.append(kContent_Type);
    // 确定文件类型
    ret.append(getFileType(m_name));
    ret.append(kContent_Type_charset);
    ret.append("\r\n");

    int beg = ret.size();
    ret.resize(f.size()+beg);
    m_fin.read(&ret[beg], f.size());
    return ret;
}
////////////////////////////////////////////////////////////////////
std::string Post::getResponse(const std::string &buff)
{
    if (m_http.parser(buff) == false)
    {
        // 可能要加
        return kHead_404;
    }
}
////////////////////////////////////////////////////////////////////
} 