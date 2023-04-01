#include "_log.h"
#include <cassert>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace dya
{
////////////////////////////////////////////////////////////////////
Logger::Logger(const char *fileName, bool sycn, int maxLine, int maxSize,  int level)
: m_fileName(fileName), m_level(level), m_maxSize(maxSize), m_sycn(sycn), m_fp(nullptr), m_maxLine(maxLine) 
,m_writeThread([this]{
    std::string buff;
    while (this->m_enable)
    {
        buff = this->m_queue.pop();
        fprintf(this->m_fp, "%s", buff.c_str());
        fflush(this->m_fp);

        // 写完后判断当前日志文件是否已经超过最大大小
        if (ftell(m_fp) > m_maxSize*1024)
            replaceFile();
    }
})
{
    getBeiJingTime();

    std::stringstream ss;
    ss << std::put_time(m_BeiJingTime, "%Y%m%d%H%M%S");
    std::string fullFileName;
    fullFileName = m_fileName + "_" + ss.str() + ".log";

    m_fp = fopen(fullFileName.c_str(), "a");
    assert(m_fp!=nullptr);
}

Logger::~Logger()
{
    fclose(m_fp);
    m_enable = false;
    m_queue.destroy();
    m_writeThread.join();
}

std::string Logger::getBeiJingTime()
{
    auto now = std::chrono::system_clock::now();
    auto t_now = std::chrono::system_clock::to_time_t(now);
    // localtime在<iomanip>头文件
    m_BeiJingTime = std::localtime(&t_now);
    std::stringstream ss;
    // put_time在<iomanip>头文件
    ss << std::put_time(m_BeiJingTime, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Logger::replaceFile()
{
    fclose(m_fp);
    m_fp = nullptr;
    getBeiJingTime();

    std::stringstream ss;
    ss << std::put_time(m_BeiJingTime, "%Y%m%d%H%M%S");
    std::string fullFileName;
    fullFileName = m_fileName + "_" + ss.str() + ".log";

    m_fp = fopen(fullFileName.c_str(), "a");
    assert(m_fp!=nullptr);
}

void Logger::setLevel(int level)
{
    m_level = level;
}

void Logger::setMaxSize(int maxSize)
{
    m_maxSize = maxSize;
}
////////////////////////////////////////////////////////////////////
}