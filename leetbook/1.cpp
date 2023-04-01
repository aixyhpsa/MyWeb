#include <iostream>
#include <vector>
//两数之和
//输入：nums = [2,7,11,15], target = 9
//输出：[0,1]
//解释：因为 nums[0] + nums[1] == 9 ，返回 [0, 1] 。

class Solution
{
public :
    std::vector<int> twoSum(std::vector<int>& nums, int target)
    {
        std::vector<int> result;
        for(int i=0; i<nums.size(); i++)
        {
            for(int j=i+1; j<nums.size(); j++)
            {
                if ( (nums[i]+nums[j]) == target)
                {
                    result.emplace_back(i);
                    result.emplace_back(j);
                    i = nums.size();
                    j = nums.size();
                }
            }
        }
        return result;
    }
};

int main()
{
    std::vector<int> nums{7, 8, 9, 10};
    Solution s1;
    auto result = s1.twoSum(nums, 15);
    for(auto &it : result)
    {
        std::cout << it << "\n";
    }
    return 0;
}