#include "_ini.h"   
namespace dya
{
Value::operator bool()
{
    return m_value == "true" ? true : false;
}
Value::operator int()
{
    return atoi(m_value.c_str());
}
Value::operator double()
{
    return atof(m_value.c_str());
}
Value::operator std::string()
{
    return m_value;
}
}