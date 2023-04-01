#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>

template<class T1, class T2>
using umap = std::unordered_map<T1, T2>;

class Solution {
public:
    int lengthOfLongestSubstring(std::string s) 
    {
        umap<char, bool> result;
        std::queue<char> q;
        int sum = 0;
        for(auto &it : s)
        {
            //看看哈希表有没有
            if (result.count(it)==0)
            {
                result[it] = 0;
                q.emplace((char)it);
                continue;
            }
            //abcdb 遍历到b的时候发现重复了，需要把哈希表中的b以及b前面的所有元素都删除
            sum = result.size()>sum ? result.size() : sum;
            while(true)
            {
                if (it == q.front()) 
                {
                    result.erase(q.front());
                    q.pop();
                    break;
                }
                result.erase(q.front());
                q.pop();
            }
            //删除老的b后，再把新的b插入哈希表
            result[it] = 0;
            q.emplace((char)it);
        }
        sum = result.size()>sum ? result.size() : sum;
        return sum;
    }
};

int main()
{
    Solution s;
    int ref = s.lengthOfLongestSubstring(std::string("aabaabcbb"));
    std::cout << ref << "\n";
    return 0;
}

