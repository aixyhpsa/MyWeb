#include "_directory.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <cassert>

namespace dya
{
////////////////////////////////////////////////////////////////////
Directory::Directory(const std::string &pathname, int flag)
: m_dp(nullptr), m_flag(flag), m_path(pathname)
{
    m_dp = opendir(pathname.c_str());
    assert(m_dp != nullptr);

    if (m_path[m_path.size()-1] != '/')
        m_path.append("/");
}

Directory::Directory(const std::string &pathname) 
: Directory(pathname, RECURSION) 
{
}

Directory::~Directory()
{
    if (m_dp != nullptr)
    {
        closedir(m_dp);
        m_dp = nullptr;
    }
}

void Directory::readdir(DIR *dp)
{
    // 判断当前遍历的是根目录还是子目录
    if (dp == nullptr)
        dp = m_dp;

    struct dirent *temp = nullptr;
    while (true)
    {
        temp = ::readdir(dp); // 步骤A
        if (temp == nullptr)
            break;
        if (temp->d_type == 4 && m_flag == RECURSION)
        {
            if (strcmp(temp->d_name, ".") == 0)
                continue;
            if (strcmp(temp->d_name, "..") == 0)
                continue;
            int size = m_path.size();
            // 除非在步骤A和步骤B之间有另外的进程或线程把目录名字修改了
            // 否则m_path一定存在，opendir一定成功
            m_path.append(temp->d_name);
            // 递归的方式遍历子目录
            this->readdir(opendir(m_path.c_str())); // 步骤B
            m_path.erase(size);
        }
        m_ret.push_back(temp);
    }

    // 关闭子目录，根目录由析构函数关闭
    if (dp != m_dp)
        closedir(dp);
}

std::string Directory::type(int index)
{
    if (m_ret[index]->d_type == 4)
        return "dir";
    return "file";
}

const char* Directory::name(int index)
{
    return m_ret[index]->d_name;
}

uint64_t Directory::size(int index)
{
    return m_ret[index]->d_reclen;
}

int Directory::count()
{
    return m_ret.size();
}
////////////////////////////////////////////////////////////////////
File::File(const char *name) : m_name(name), m_buff(nullptr) {}

File::~File()
{
    if (m_buff != nullptr)
        delete m_buff;
}

bool File::isExist()
{
    return access(m_name, F_OK) == 0 ? true : false;
}

bool File::isDir()
{
    return S_ISDIR(this->mode());
}

bool File::isReg()
{
    return S_ISREG(this->mode());
}

int64_t File::size()
{
    assert(this->stat());
    return m_buff->st_size;
}
uint32_t File::mode()
{
    assert(this->stat());
    return m_buff->st_mode;
}
int64_t File::atime()
{
    assert(this->stat());
    return m_buff->st_atime;
}
int64_t File::ctime()
{
    assert(this->stat());
    return m_buff->st_ctime;
}
int64_t File::mtime()
{
    assert(this->stat());
    return m_buff->st_mtime;
}

bool File::rename(const char *newName)
{
    return ::rename(m_name, newName) == 0 ? true : false;
}

bool File::remove()
{
    return ::remove(m_name) == 0 ? true : false;
}

bool File::stat()
{
    if (m_buff == nullptr)
    {
        m_buff = new struct stat();
        if (this->isExist() == false)
            return false;
        bool ret = ::stat(m_name, m_buff) == 0 ? true : false;
        return ret;
    }
    return true;
}
////////////////////////////////////////////////////////////////////
}