#include <iostream>
#include <string>
#include <unordered_map>

template<class T1, class T2>
using umap = std::unordered_map<T1, T2>;

class Solution {
public:
    std::string intToRoman(int num) 
    {
        umap<int, char> numList(16);
        numList.insert({{1, 'I'}, {5, 'V'}, {10, 'X'}, {50, 'L'}, {100, 'C'}, {500, 'D'}, {1000, 'M'}});
        std::string result;
        auto it = numList.begin();
        for(; it!=numList.end(); it++)
        {
            if(num-(it->first) >= 0)
                break;
        }
        while(num > 0)
        {
            if(num-(it->first) >= 0)
            {
                num = num-(it->first);
                result += it->second;
            }
            else
            {
                it++;
            }
        }
        int ptr = 0;
        //900
        if ((ptr = result.find("DCCCC")) != -1)
            result.replace(ptr, 5, "CM");
        //400
        if ((ptr = result.find("CCCC"), ptr) != -1)
            result.replace(ptr, 4, "CD");
        //90
        if ((ptr = result.find("LXXXX"), ptr) != -1)
            result.replace(ptr, 5, "XC");
        //40
        if ((ptr = result.find("XXXX"), ptr) != -1)
            result.replace(ptr, 4, "XL");
        //9
        if ((ptr = result.find("VIIII"), ptr) != -1)
            result.replace(ptr, 5, "IX");
        //4
        if ((ptr = result.find("IIII"), ptr) != -1)
            result.replace(ptr, 4, "IV");
        return result;
    }
};

int main()
{
    Solution ss;
    std::string &&result = ss.intToRoman(1994);
    std::cout << "rst=" << result << "\n";
}