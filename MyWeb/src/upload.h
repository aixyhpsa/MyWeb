#pragma once
#include <string>

namespace dya
{
////////////////////////////////////////////////////////////////////
class Upload
{
public :

    /**
     * @brief 把网页上传的数据写入本地文件
     * @param 本地存放的目录, 文件名不用管
     * @param 待写入的内容，文件名就在该内容当中，程序会自己解析出来
     * @return bool
     */
    bool operator()(const std::string &uploadPath, const std::string &body);
};
////////////////////////////////////////////////////////////////////
}