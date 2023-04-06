#pragma once   
#include <string>

namespace dya
{
////////////////////////////////////////////////////////////////////
class Time
{
public :

    /**
     * @brief 将UTC时间(即time(0))转换成字符串
     * @param time_t
     * @param 转换的格式，例如："%Y-%m-%d %H:%M:%S"
     * @return std::string
     */
    static std::string to_string(time_t UTC, const char *format);
};
////////////////////////////////////////////////////////////////////
}