#pragma once
#include <fstream>
#include <string>

namespace dya
{
////////////////////////////////////////////////////////////////////
class Flow
{
public :
    Flow(std::string &name);
    Flow(std::string &&name);
    std::string handle();
private :
    std::string getFileType();
private :
    std::ifstream m_fin;
    std::string m_buff;
};
////////////////////////////////////////////////////////////////////
} 
