#pragma once
#include <fstream>
#include <string>
#include "_directory.h"

namespace dya
{
////////////////////////////////////////////////////////////////////
class Upload
{
public :
    Upload(const std::string &path);
    int write(const char *fileName, const char *content);
private :
    std::ofstream m_fout;
    std::string m_path;
};
////////////////////////////////////////////////////////////////////
class ShowList
{
public :
    ShowList(const char *path);
    std::string show();
private :
    Dir m_dir;
};
////////////////////////////////////////////////////////////////////
/*
class ShowList
{
public :
    ShowList(const char *path);
    Json::JSON getList();
private :
    Dir m_dir;
};
*/
////////////////////////////////////////////////////////////////////
}