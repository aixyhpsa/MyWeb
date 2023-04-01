#pragma once
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <iostream>
namespace dya
{

    class Value
    {
    private :
        std::string m_value;
    public :
        Value() = default;
        ~Value() = default;
        
        //把bool、int、double、char *、string统统转换成string
        template<typename T>
        Value(T && value)
        {
            *this = std::forward<T>(value);
        }

        //使用stringstream无脑转换
        template<typename T>
        Value & operator=(T&& value)
        {
            std::stringstream ss;
            ss << std::boolalpha;
            ss << value;
            m_value = ss.str();
            return *this;
        }
    public :
        operator bool();
        operator int();
        operator double();
        operator std::string();
    };

    class Section
    {
    private :
        std::map<std::string, Value> m_section;
    public :
        Section() = default;
        ~Section() = default;
    };

    class IniFile
    {
    private :
        std::map<std::string, Section> m_result;
        std::ifstream m_fin;
    public :
        IniFile() = default;
        ~IniFile() = default;

        template<typename T>
        bool load(T &&filename)
        {
            m_fin.open(filename);
            if (m_fin.fail())
                return false;
            std::string buff;
            while(std::getline(m_fin, buff))
            {
                //puts(buff.c_str());
                if (buff == "")
                    continue;
                if (buff[0] == '[')
                {
                    auto end = buff.find_first_of("]");
                    //m_result[buff.substr(1, end)] = Section();
                    std::cout << buff.substr(1, end) << "\n";
                }
            }
            return true;
        }
               
    };

}
