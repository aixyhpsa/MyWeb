    //#pragma once
#include <string>
#include <vector>

namespace dya
{
    class Value
    {
    public :
        union Content
        {
            bool *m_bool;
            int *m_int;
            double *m_double;
            std::string *m_string;
            std::vector<int> *m_vector;
            Value *m_obj;
        };
        enum Type
        {
            json_null,
            json_bool,
            json_int,
            json_double,
            json_string,
            json_vector,
            json_obj
        };
    public :
        Value() = default;
        ~Value() = default;
        
        template<typename T>
        Value(T && value)
        {
            *this = std::forward<T>(value);
        }

        template<typename T>
        Value & operator=(T&& value)
        {
            if (typeid(value) == typeid(bool))
                m_content.m_bool = reinterpret_cast<bool *>(&value);
            else if (typeid(value) == typeid(int))
                m_content.m_int =  reinterpret_cast<int *>(&value);
            else if (typeid(value) == typeid(double))
                m_content.m_double= reinterpret_cast<double *>(&value);
            else if (typeid(value) == typeid(std::string))
                m_content.m_string= reinterpret_cast<std::string *>(&value);
            else if (typeid(value) == typeid(std::vector<int>))
                m_content.m_vector= reinterpret_cast<std::vector<int> *>(&value);
            else if (typeid(value) == typeid(Value))
                m_content.m_obj= reinterpret_cast<Value *>(&value);
            return *this;
        }
    //private :
        Type m_type;       
        Content m_content;
    public :
/*         operator bool();
        operator int();
        operator double();
        operator std::string(); */
    };
}