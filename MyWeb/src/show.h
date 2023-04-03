#pragma once
#include <fstream>
#include <string>
#include "../tools/_directory.h"
#include "nlohmann/json.hpp"

namespace dya
{
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
class ShowFileList 
{
public :
    std::string operator()(const std::string &name);
private :
    void addJsonField(nlohmann::json &nj, File &f);
};
////////////////////////////////////////////////////////////////////
}