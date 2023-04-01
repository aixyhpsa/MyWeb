#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>


class Solution {
public:
    double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2)
    {
        //nums1.insert(nums1.end(), std::make_move_iterator(nums2.begin()), std::make_move_iterator(nums2.end()));
        nums1.insert(nums1.end(), nums2.begin(), nums2.end());
        std::sort(nums1.begin(), nums1.end());

        int middle = nums1.size()/2;
        if (nums1.size()%2 == 1)
            return nums1[middle];

        return (double)(nums1[middle]+nums1[middle-1])/2;
    }
};    

     
int main()
{
    Solution ss;
    std::vector<int> v1{1, 4, 8, 0};
    std::vector<int> v2{2, 7, 11, 15};
    auto result = ss.findMedianSortedArrays(v1, v2);
    std::cout << "result = " << result << "\n";
    return 0;
}