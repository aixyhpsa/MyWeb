#include "_directory.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

namespace dya
{
////////////////////////////////////////////////////////////////////
File::File(std::string &name) : m_name(name), m_buff(nullptr) {}

File::File(std::string &&name) : m_name(std::move(name)) {}

File::~File()
{
    if (m_buff != nullptr)
    {
        delete m_buff;
        m_buff = nullptr;
    }
}

bool File::isExist()
{
    return access(m_name.c_str(), F_OK) == 0 ? true : false;
}

bool File::isExist(const char *name)
{
    return access(name, F_OK) == 0 ? true : false;
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
    return ::rename(m_name.c_str(), newName) == 0 ? true : false;
}

bool File::remove()
{
    return ::remove(m_name.c_str()) == 0 ? true : false;
}

bool File::stat()
{
    if (m_buff == nullptr)
    {
        m_buff = new struct stat();
        if (this->isExist() == false)
            return false;
        bool ret = ::stat(m_name.c_str(), m_buff) == 0 ? true : false;
        return ret;
    }
    return true;
}

const char* File::name()
{
    return m_name.c_str();
}
////////////////////////////////////////////////////////////////////
Dir::~Dir()
{
    if (m_dp != nullptr)
        closedir(m_dp);
}

void Dir::readdir()
{
    struct dirent *temp = nullptr;
    while (true)
    {
        temp = ::readdir(m_dp); // 步骤A
        if (temp == nullptr)
            break;
        if (strcmp(temp->d_name, ".") == 0)
                continue;
        if (strcmp(temp->d_name, "..") == 0)
                continue;

        int end = m_path.size();
        m_path.append(temp->d_name);
        m_files.emplace_back(m_path);
        m_path.erase(end);
    }
}

File& Dir::operator[](int index)
{
    return m_files[index];
}

int Dir::count()
{
    return m_files.size();
}
////////////////////////////////////////////////////////////////////
}