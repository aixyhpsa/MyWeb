#pragma once
#include <fstream>
#include <string>

namespace dya
{
////////////////////////////////////////////////////////////////////
const std::string kRootPath{"/home/wucz/test/MyWeb/resource"};
const std::string kUploadPath{"/home/wucz/test/MyWeb/resource/upload"};
////////////////////////////////////////////////////////////////////
class Get 
{
public :
    Get(std::string &name);
    Get(std::string &&name);
    std::string getResponse();
private :
    std::string getFileType();
private :
    std::ifstream m_fin;
    std::string m_name;
};
////////////////////////////////////////////////////////////////////
class Post
{
public :

    /**
     * @brief 处理POST请求
     * @param HTTP请求的请求体
     */
    Post(const std::string &buff);

    std::string getResponse();
private :
    std::string error();
    std::string success(const std::string &srsponseBody);
private :
    const std::string &m_buff;
};
////////////////////////////////////////////////////////////////////
} 
