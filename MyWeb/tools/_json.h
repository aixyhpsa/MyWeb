#pragma once
#include "yyjson.h"
#include <memory>
#include <string>
#include <vector>

namespace dya
{
namespace Json
{
using JSON = std::shared_ptr<char[]>;
////////////////////////////////////////////////////////////////////
class Write 
{
public :
    Write();
    ~Write();
    bool add(const char *key, const char *value);
    bool add(const char *key, int value);
    bool add(const char *key, int *value, int len);
    bool add(const char *key, const std::vector<std::string> &value);
    bool add(const char *key, const std::vector<JSON> &value);
    JSON get();
private :
    yyjson_mut_doc *m_doc;
    yyjson_mut_val *m_root;
};
////////////////////////////////////////////////////////////////////
}
}