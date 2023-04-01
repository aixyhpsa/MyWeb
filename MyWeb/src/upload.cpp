#include <cassert>
#include <sstream>
#include <vector>
#include "upload.h"
#include "_time.h"
#include "nlohmann/json.hpp"

namespace dya
{
////////////////////////////////////////////////////////////////////
Upload::Upload(const std::string &path) : m_path(path)
{
    File f(path.c_str());
    assert(f.isExist());

    if (m_path[m_path.size()-1] != '/')
        m_path.append("/");
}

int Upload::write(const char *fileName, const char *content)
{
    m_fout.open((m_path+fileName), std::ios::binary);
    m_fout << content;
    m_fout.close();
    return 0;
}
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
/*使用yyjson的版本
ShowList::ShowList(const char *path) : m_dir(path) {}

Json::JSON ShowList::getList()
{
    m_dir.readdir();
    std::vector<Json::JSON> vs;
    for (int i=0; i<m_dir.count(); i++)
    {
        Json::Write json;
        json.add("name", m_dir[i].name());
        json.add("size", m_dir[i].size());
        std::string date = Time::to_string(m_dir[i].mtime(), "%Y-%m-%d %H:%M:%S");
        json.add("date", date.c_str());
        if (m_dir[i].isDir())
            json.add("type", "dir");
        else
            json.add("type", "file");
        vs.emplace_back(json.get());

    }
    Json::Write json;
    json.add("result", vs);
    return json.get();
}
*/
////////////////////////////////////////////////////////////////////
}