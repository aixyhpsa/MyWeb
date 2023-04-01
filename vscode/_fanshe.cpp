#include "_fanshe.h"

namespace dya
{
    void ClassFactory::register_class(const std::string &className, std::function<void*()> method)
    {
        m_classMap[className] = method;
    }

    void * ClassFactory::create_class(const std::string &className)
    {
        auto result = m_classMap.find(className);
        if (result != m_classMap.end())
            return m_classMap[className]();
        else
            return nullptr;
    }
}