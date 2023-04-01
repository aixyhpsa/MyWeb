/**
 * @file _log.h
 * @brief 日志系统
 * @author dya
 * @date 2023-03-23
 */    
#pragma once
#include <string>
#include <fstream>
#include <thread>
#include "_mqueue.hpp"
#include "_singleton.hpp"

namespace dya
{
////////////////////////////////////////////////////////////////////
constexpr char kLOG_LEVEL[5][6] = {"DEBUG", "INFO ", "WARN ", "ERROR", "FATAL"};

#define LOG_INIT(...) dya::Singleton<dya::Logger>::instance(__VA_ARGS__)

#define LOG_SET_LEVEL(x) dya::Singleton<dya::Logger>::getInstance()->setLevel(x)

#define LOG_SET_FILEMAX(x) dya::Singleton<dya::Logger>::getInstance()->setMaxSize(x)

#define LOG_DEBUG(format, ...) \
dya::Singleton<dya::Logger>::getInstance()->write(dya::Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define LOG_INFO(format, ...) \
dya::Singleton<dya::Logger>::getInstance()->write(dya::Logger::INFO,  __FILE__, __LINE__, format, ##__VA_ARGS__)

#define LOG_WARN(format, ...) \
dya::Singleton<dya::Logger>::getInstance()->write(dya::Logger::WARN,  __FILE__, __LINE__, format, ##__VA_ARGS__)

#define LOG_ERROR(format, ...) \
dya::Singleton<dya::Logger>::getInstance()->write(dya::Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define LOG_FATAL(format, ...) \
dya::Singleton<dya::Logger>::getInstance()->write(dya::Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)
////////////////////////////////////////////////////////////////////
class Logger
{
public :   
    friend Singleton<Logger>;

    // 日志等级
    enum { DEBUG, INFO,  WARN, ERROR, FATAL};

    /**
     * @brief 向文件里写入一行日志，根据成员变量m_sync决定写入模式
     * param[in] 写入时的日志等级，使用上文的宏定义，不用手动指定
     * param[in] 哪个文件要写入，使用上文的宏定义，不用手动指定
     * param[in] 哪个文件的哪一行要写入，使用上文的宏定义，不用手动指定
     * param[in] 写入时的格式
     * param[in] 格式需要的参数（该参数是可变长参数）
     * @return void
     */
    template<typename... Args>
    void write(int level, const char *file, int line, const char *format, Args&&... args);

    void setLevel(int);

    void setMaxSize(int);
private :
    /**
     * @brief 构造函数，日志系统初始化
     * param[in] 日志文件名称
     * param[in] 日志写入方式
     * param[in] 单行日志最大长度
     * param[in] 单个日志文件最大大小(单位：k)
     * param[in] 初始日志等级
     */
    Logger(const char* fileName, bool sycn = false, int maxLine = 120, int maxSize = 100,  int level = DEBUG);

    virtual ~Logger();

    // 类内使用，类外不使用
    std::string getBeiJingTime();

    // 类内使用，类外不使用
    void replaceFile();
private :
    bool m_sycn;                        // true：同步写入；false：异步写入
    int m_level;                        // 当前日志等级
    int m_maxSize;                      // 单个日志文件的最大大小
    const int m_maxLine;                // 单行日志最大长度
    bool m_enable = true;               // 写线程退出循环标志
    std::thread m_writeThread;          // 写线程，一般只需要一个
    MessageQueue<std::string> m_queue;  // 消息队列，异步写入时才会使用，否则不使用
    std::string m_fileName;             // 日志文件名
    FILE *m_fp;                         // 文件指针
    tm *m_BeiJingTime;                  // 北京时间
};
////////////////////////////////////////////////////////////////////
template<typename... Args>
void Logger::write(int level, const char *file, int line, const char *format, Args&&... args)
{
    if (level < m_level || level > 4)
        return;

    // 同步写入方式，即直接写入文件
    if (m_sycn)
    {
        fprintf(m_fp, "%s %s %s %d ", getBeiJingTime().c_str(), &kLOG_LEVEL[level][0], file, line);
        fprintf(m_fp, format, std::forward<Args>(args)...);
        fflush(m_fp);

        // 写完后判断当前日志文件是否已经超过最大大小
        if (ftell(m_fp) > m_maxSize*1024)
            replaceFile();
        return;
    }

    // 异步写入方式，即将日志添加到消息队列，由写线程取出日志并写入文件
    char buff[m_maxLine]{0};
    int ret = sprintf(buff, "%s %s %s %d ", getBeiJingTime().c_str(), &kLOG_LEVEL[level][0], file, line);
    snprintf(&buff[ret], m_maxLine-ret, format, std::forward<Args>(args)...);
    m_queue.push(buff);
}
////////////////////////////////////////////////////////////////////
}