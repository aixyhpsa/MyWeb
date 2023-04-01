#pragma once
#include <sys/stat.h>
#include <dirent.h>
#include <memory>
#include <string>
#include <vector>
#include <cassert>

namespace dya 
{
////////////////////////////////////////////////////////////////////
class Dir;
class File
{
public :
    File(std::string &name);
    File(std::string &&name);

    ~File();

    /**
     * @brief 判断文件是否存在
     * @return bool
     */
    bool isExist();

    /**
     * @brief 不需要实例化对象，判断一个文件是否存在
     * @return bool
     */
    static bool isExist(const char *name);

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

    const char* name();
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
    std::string m_name;
    struct stat *m_buff;
};
////////////////////////////////////////////////////////////////////
class Dir
{
public :
    /**
     * @brief 构造函数
     * @param 输入目录的路径，如果是左值那么拷贝，如果是右值那么移动
     * @return
     */
    template<typename T>
    Dir(T&& path);

    ~Dir();

    void readdir();
    File& operator[](int index);
    int count();
private :
    std::vector<File> m_files; 
    DIR *m_dp;
    std::string m_path;
};
////////////////////////////////////////////////////////////////////
template<typename T>
Dir::Dir(T&& path) : m_dp(nullptr), m_path(std::forward<T>(path))
{
    m_dp = opendir(m_path.c_str());
    assert(m_dp != nullptr);

    if (m_path[m_path.size()-1] != '/')
        m_path.append("/");
}
////////////////////////////////////////////////////////////////////
} 
