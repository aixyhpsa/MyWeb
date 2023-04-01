#include <iostream>
#include <string>
#include <vector>
const std::vector<std::string> numList{"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

class Solution {
public:
    std::vector<std::string> letterCombinations(std::string digits) 
    {
        if (digits.size() == 0)
            return std::vector<std::string>();

        std::string s = numList[digits[0]-48];
        std::vector<std::string> result;

        if (digits.size() == 1)
        {
            for(int i=0; i<s.size(); i++)
            {
                result.emplace_back(s.substr(i, 1));
            }
            return result;
        }
        //进入递归
        auto subResult = letterCombinations(digits.substr(1, digits.size()));
        //s X subResult
        for(int i=0; i<s.size(); i++)
        {
            for(int j=0; j<subResult.size(); j++)
            {
                result.emplace_back(s[i]+subResult[j]);
            }
        }
        return result;
    }
};

int main()
{
    Solution ss;
    std::string input;
    std::cin >> input;
    auto result = ss.letterCombinations(input);
    for(const auto &it : result)
    {
        std::cout << it << "\n";
    }
}