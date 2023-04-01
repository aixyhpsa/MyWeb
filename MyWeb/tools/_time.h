#pragma once   
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

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

std::string Time::to_string(time_t UTC, const char *format)
{
    auto localTime = std::localtime(&UTC);
    std::stringstream ss;
    ss << std::put_time(localTime, format);
    return ss.str();
}
////////////////////////////////////////////////////////////////////
}