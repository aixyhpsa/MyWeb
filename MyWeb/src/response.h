#pragma once
#include <fstream>
#include <string>

namespace dya
{
////////////////////////////////////////////////////////////////////
const char kRootPath[] = "/home/wucz/test/MyWeb/resource";
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
    Post(const std::string &buff);

    std::string getResponse();
private :
    std::string upload();
    std::string error();
private :
    const std::string &m_buff;
};
////////////////////////////////////////////////////////////////////
} 
