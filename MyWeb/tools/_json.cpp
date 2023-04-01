#include "_json.h"

namespace dya
{
namespace Json
{
////////////////////////////////////////////////////////////////////
Write::Write()
{
    m_doc = yyjson_mut_doc_new(NULL);
    m_root = yyjson_mut_obj(m_doc);
    yyjson_mut_doc_set_root(m_doc, m_root);
}

Write::~Write()
{
    yyjson_mut_doc_free(m_doc);
}

bool Write::add(const char *key, const char *value)
{
    return yyjson_mut_obj_add_str(m_doc, m_root, key, value);
}

bool Write::add(const char *key, int value)
{
    return yyjson_mut_obj_add_int(m_doc, m_root, key, value);
}

bool Write::add(const char *key, int *value, int len)
{
    yyjson_mut_val *vs = yyjson_mut_arr_with_sint32(m_doc, value, len);
    return yyjson_mut_obj_add_val(m_doc, m_root, key, vs);
}

bool Write::add(const char *key, const std::vector<std::string> &value)
{
    yyjson_mut_val *arr = yyjson_mut_arr(m_doc);
    for (auto &it : value)
    {
        yyjson_mut_arr_add_str(m_doc, arr, it.c_str());
    }
    return yyjson_mut_obj_add_val(m_doc, m_root, key, arr);
}

bool Write::add(const char *key, const std::vector<JSON> &value)
{
    yyjson_mut_val *arr = yyjson_mut_arr(m_doc);
    for (auto &it : value)
    {
        yyjson_mut_arr_add_str(m_doc, arr, it.get());
    }
    return yyjson_mut_obj_add_val(m_doc, m_root, key, arr);   
}

JSON Write::get()
{
    char *json = yyjson_mut_write(m_doc, 0, NULL);
    return std::shared_ptr<char[]> (json);
}
////////////////////////////////////////////////////////////////////
}
}