#pragma once
#include <map>
#include <string>
#include <functional>
namespace dya
{
    class ClassFactory
    {
    public :
    //method:方法
        void register_class(const std::string &className, std::function<void*()> method);
        void * create_class(const std::string &className);
    private :
        std::map<std::string, std::function<void*()>> m_classMap;
    };

    class ClassRegister
    {
    public :
        ClassRegister(const std::string &className, std::function<void*()> method)
        {
            ClassFactory cf;
            cf.register_class(className, method);
        }
    };
    //宏定义中#x:把x的值当成字符串,##x:把x的当成字符串并且和前文拼接
    #define REGISTER_CLASS(x) \
        ClassRegister cr##x(#x, []()->void*{ void *obj = new x; return obj})
}