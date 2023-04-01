#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "http_parser.h"

namespace dya
{
////////////////////////////////////////////////////////////////////
template<class T1, class T2>
using umap = std::unordered_map<T1, T2>;
////////////////////////////////////////////////////////////////////
class Http
{
public :
    Http();
    ~Http();

    /**
     * @brief 默认解析http请求报文；使用reset()可以更改报文类型。
     * @param 待解析字符串
     * @return bool
     */
    bool parser(const std::string&); 

    /**
     * @brief 清空上次解析记录，并且可以指定下次解析的报文类型
     * @param 宏：HTTP_REQUEST代表请求报文；宏：HTTP_RESPONSE代表响应报文
     * @return void
     */
    void reset(int type=HTTP_REQUEST);

    /**
     * @brief 解析失败时的错误提示
     * @return const char*
     */
    const char* error();

    std::string& operator[](const std::string &key);
    bool find(const std::string &key);
private :
    int on_message_begin(http_parser* _);
    int on_headers_complete(http_parser* _);
    int on_message_complete(http_parser* _);
    
    int on_url(http_parser* _, const char* at, size_t length);
    int on_header_field(http_parser* _, const char* at, size_t length);
    int on_header_value(http_parser* _, const char* at, size_t length);
    int on_body(http_parser* _, const char* at, size_t length);
    int on_status(http_parser* _, const char* at, size_t length);
public :
    umap<std::string, std::string> m_result;
private :
    std::unique_ptr<struct http_parser_settings> m_setting;
    std::unique_ptr<struct http_parser> m_parser;
    const char *m_errno;
};
////////////////////////////////////////////////////////////////////
}