#pragma once
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <vector>

namespace dya 
{
////////////////////////////////////////////////////////////////////
class Directory
{
public :
    enum { RECURSION, NON_RECURSION};
    /**
     * @brief 构造函数
     * @param 目录的路径
     * @param 是否遍历该目录下的子目录，RECURSION：是；NON_RECURSION：否。
     * @return
     */
    Directory(const std::string &pathname, int flag);
    Directory(const std::string &pathname);
    ~Directory();

    void readdir(DIR *dp = nullptr);
    std::string type(int index);
    const char* name(int index);
    std::string date(int index);
    uint64_t size(int index);
    int count();
private :
    std::vector<struct dirent*> m_ret;
    //std::vector<Directory> 
    int m_flag;
    DIR *m_dp;
    std::string m_path;
};
////////////////////////////////////////////////////////////////////
class File
{
public :
    File(const char *name);

    ~File();

    /**
     * @brief 判断文件是否存在
     * @return bool
     */
    bool isExist();

    /**
     * @brief 判断是否是目录
     * @return bool
     */
    bool isDir();

    /**
     * @brief 判断是否是一般文件
     * @return bool
     */
    bool isReg();

    /**
     * @brief 返回文件大小
     * @return int64_t
     */
    int64_t size();

    /**
     * @brief 文件最近一次被存取或被执行的时间。
     * @brief utime、read、write与tructate时改变。
     * @return int64_t
     */
    int64_t atime();

    /**
     * @brief 文件最后一次被修改的时间。
     * @brief mknod、utime和write时才会改变。
     * @return int64_t
     */
    int64_t mtime();

    /**
     * @brief 最近一次被更改的时间。
     * @brief 此参数会在文件所有者、组、权限被更改时更新。
     * @return int64_t
     */
    int64_t ctime();

    /**
     * @brief 重命名文件
     * @param 新文件名
     * @return bool
     */
    bool rename(const char *newName);

    /**
     * @brief 删除文件
     * @return bool
     */
    bool remove();
private :
    /**
     * @brief 文件的类型和存取的权限。
     * @brief 使用宏S_ISREG(st_mode)判断是否为一般文件
     * @brief 使用宏S_ISDIR(st_mode)判断是否为目录
     * @return uint32_t
     */
    uint32_t mode();

    bool stat();
private :
    const char *m_name;
    struct stat *m_buff;
};
////////////////////////////////////////////////////////////////////
} 
