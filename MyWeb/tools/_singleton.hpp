#pragma once
#include "_nocopyable.h"

namespace dya
{
//////////////////////////////////////////////////////////////
template<class T>
class Singleton : public NonCopyable
{
public :
    template<typename... Args>
    static T* instance(Args&&... args);

    static T* getInstance();

    static void destroy();
private :
    Singleton();
    virtual ~Singleton();
private :
    static T *m_ptr;
};
//////////////////////////////////////////////////////////////
template<class T> T* Singleton<T>::m_ptr = nullptr;

template<class T>
template<typename... Args>
T* Singleton<T>::instance(Args&&... args)
{
    if (m_ptr == nullptr)
        m_ptr = new T(std::forward<Args>(args)...);
    return m_ptr;
}

template<class T>
T* Singleton<T>::getInstance()
{
    //if (m_ptr == nullptr)
    return m_ptr;
}

template<class T>
void Singleton<T>::destroy()
{
    delete m_ptr;
    m_ptr = nullptr;
}
//////////////////////////////////////////////////////////////
}