#include <sstream>
#include "show.h"
#include "_time.h"

namespace dya
{
////////////////////////////////////////////////////////////////////
// 使用nlohmann的版本
ShowList::ShowList(const char *path) : m_dir(path) {}

std::string ShowList::show()
{
    m_dir.readdir();
    std::stringstream ss;
    nlohmann::json nj;
    for (int i=0; i<m_dir.count(); ++i)
    {
        std::string date = Time::to_string(m_dir[i].mtime(), "%Y-%m-%d %H:%M:%S");
        char type[5]{0};
        if (m_dir[i].isDir())
            strcpy(type, "dir");
        else
            strcpy(type, "file");

        nj[m_dir[i].name()] = {{"size", m_dir[i].size()}, 
                               {"date", date.c_str()}, 
                               {"typ", type}};
    }
    ss << nj;
    return ss.str();
}
////////////////////////////////////////////////////////////////////
std::string ShowFileList::operator()(const std::string &name)
{
    File f(const_cast<std::string&>(name));
    if (f.isExist() == false)
        return {};
    if (f.isDir())
    {
        Dir dir(name);
        dir.readdir();
        std::stringstream ss;
        nlohmann::json nj;
        for (int i=0; i<dir.count(); ++i)
        {
            this->addJsonField(nj, dir[i]);
        }
        ss << nj;
        return ss.str();
    }

    if (f.isReg())
    {
        std::stringstream ss;
        nlohmann::json nj;
        this->addJsonField(nj, f);
        ss << nj;
        return ss.str();
    }
    return {};
}

void ShowFileList::addJsonField(nlohmann::json &nj, File &f)
{
    std::string date = Time::to_string(f.mtime(), "%Y-%m-%d %H:%M:%S");
    char type[5]{0};
    if (f.isDir())
        strcpy(type, "dir");
    else
        strcpy(type, "file");

    nj[f.name()] = {{"size", f.size()}, {"date", date.c_str()}, {"typ", type}};   
}
////////////////////////////////////////////////////////////////////
}