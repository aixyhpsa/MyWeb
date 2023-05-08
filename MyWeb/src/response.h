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
    //std::string error();

    /**
     * @brief 返回失败的响应报文
     * @param 设置错误代码
     * @param 设置响应体
     */
    std::string error(int, const std::string &);

    /**
     * @brief 返回成功的响应报文，如果默认的响应头够用了，使用这个函数
     * @param 设置响应体
     */
    std::string success(const std::string &);

    /**
     * @brief 返回成功的响应报文，如果还需要添加响应头字段，使用这个函数
     * @param 可以设置若干条响应头
     * @param 设置响应体
     */
    std::string success(const std::string &, const std::string &);
private :
    const std::string &m_buff;
};
////////////////////////////////////////////////////////////////////
} 
