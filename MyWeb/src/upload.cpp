#include "upload.h"
#include <fstream>
#include "../tools/_directory.h"

namespace dya
{
////////////////////////////////////////////////////////////////////
bool Upload::operator()(const std::string &uploadPath, const std::string &body)
{
    // 从body中找到文件名称
    int beg = body.find("filename=");
    if (beg == -1)
        return false;
    beg = beg + sizeof("filename=");
    int end = body.find(R"(")", beg);
    std::string fileName = uploadPath;
    // 判断本地存放目录(uploadPath)不是 '/' 结尾
    if (fileName[fileName.size()-1] != '/')
        fileName.append("/");
    int pathCount = fileName.size();
    fileName.append(body.substr(beg, end-beg));

    // 检查这个文件是否已经存在
    if (File::isExist(fileName.c_str()) == true)
    {
        // 对付名称重复的策略：在名字的后面后缀名的前面加(1)
        int suffix = fileName.find_last_of(".", fileName.size()-1, fileName.size()-1-pathCount);
        if (suffix == -1)
            fileName.append("(1)");
        else
            fileName.insert(suffix, "(1)");
    }
    // 如果(1)还是重复，那么括号中间数字加一，以此类推，直到不再重复为止
    int count = 2;
    while (File::isExist(fileName.c_str()))
    {
        int right = fileName.find_last_of(")", fileName.size()-1, fileName.size()-1-pathCount);
        int left = fileName.find_last_of("(", fileName.size()-1, fileName.size()-1-pathCount);
        fileName.replace(left+1, right-left-1, std::to_string(count));
        ++count;
    }

    std::ofstream fout;
    fout.open(fileName, std::ios::app);
    if (fout.is_open() == false)
        return false;

    beg = body.find("\r\n", end);
    if (beg == -1)
    {
        fout.close();
        return false; 
    }
    beg += sizeof("\r\n");
    end = body.find("------WebKitFormBoundary", end);
    if (end == -1)
    {
        fout.close();
        return false;
    }
    fout << body.substr(beg, end-beg);
    fout.close();
    return true;
}
////////////////////////////////////////////////////////////////////
}