#include "_time.h"   
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

namespace dya
{
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