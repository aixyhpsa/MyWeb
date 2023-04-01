#include <iostream>
#include <vector>
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
 };
/*
ListNode* createListNode(long long sum)
{
    ListNode *result = new ListNode();
    if (sum < 0)
    {
        delete result;
        return nullptr;
    }
    else if (sum == 0)
    {
        result->val = 0;
        result->next = nullptr;
        return result;
    }
    else
    {
        result->val = sum%10;
        //result->next = createListNode(sum/10);
        result->next = (sum/10) == 0 ? nullptr : createListNode(sum/10);
    }
    return  result;
}

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        long long num1=0, num2=0, i;
        for(i=1; l1->next!=nullptr; l1=l1->next,i=i*10)
        {
            num1 = num1 + l1->val*i;
        }
        num1 = num1 + l1->val*i;
        for(i=1; l2->next!=nullptr; l2=l2->next,i=i*10)
        {
            num2 = num2 + l2->val*i;
        }
        num2 = num2 + l2->val*i;
        ListNode *result = createListNode(num1 + num2);
        return result;
    }
};
*/

ListNode* createListNode(const std::vector<int> &sum)
{
    ListNode *beg= new ListNode();
    auto temp = beg;
    for(int i=0; i<sum.size()-1; i++)
    {
        temp->val = sum[i];
        ListNode *end= new ListNode();
        temp->next = end;
        temp = temp->next;
    }
    temp->val = sum[sum.size()-1];
    temp->next = nullptr;
    return beg;
}

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        int carry = 0;
        std::vector<int> sum;
        while(l1 != nullptr || l2 != nullptr)
        {
            int temp = 0;
            if (l1 == nullptr && l2 != nullptr)
            {
                temp = l2->val + carry;
                l2 = l2->next;
            }
            else if (l1 != nullptr && l2 == nullptr)
            {
                temp = l1->val + carry;
                l1 = l1->next;
            }
            else
            {
                temp = l1->val + l2->val +carry;
                l1 = l1->next;
                l2 = l2->next;
            }

            sum.push_back(temp%10);
            //carry = temp/10;
            carry = temp > 9 ? 1 : 0;
        }
        if (carry > 0)
            sum.push_back(carry);

        auto result = createListNode(sum);
        return result;
    }
};

int main()
{
    //auto l1 = createListNode(1000000000000000000000000000001);
    //auto l2 = createListNode(465);
    //Solution sl;
    //auto result = sl.addTwoNumbers(l1, l2);

    auto l1 = createListNode(std::vector<int>({0}));
    auto l2 = createListNode(std::vector<int>({0}));
    //auto l1 = createListNode(std::vector<int>({5, 6, 4}));
    //auto l2 = createListNode(std::vector<int>({2, 4, 3}));

    Solution s;
    auto result = s.addTwoNumbers(l1, l2);
    int a=0;
}