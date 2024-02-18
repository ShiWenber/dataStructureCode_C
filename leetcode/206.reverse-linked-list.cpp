/*
 * @lc app=leetcode.cn id=206 lang=cpp
 *
 * [206] Reverse Linked List
 *
 * https://leetcode.cn/problems/reverse-linked-list/description/
 *
 * algorithms
 * Easy (73.94%)
 * Likes:    3501
 * Dislikes: 0
 * Total Accepted:    1.8M
 * Total Submissions: 2.4M
 * Testcase Example:  '[1,2,3,4,5]'
 *
 * Given the head of a singly linked list, reverse the list, and return the
 * reversed list.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: head = [1,2,3,4,5]
 * Output: [5,4,3,2,1]
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: head = [1,2]
 * Output: [2,1]
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: head = []
 * Output: []
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in the list is the range [0, 5000].
 * -5000 <= Node.val <= 5000
 * 
 * 
 * 
 * Follow up: A linked list can be reversed either iteratively or recursively.
 * Could you implement both?
 * 
 */


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
#include <iostream>

using namespace std;

class Solution {
public:

    // ListNode* reverseList(ListNode* head) {
    //   if (head == nullptr) {
    //     return head;
    //   }
    //   ListNode* q = head->next;
    //   ListNode* s = head;
    //   ListNode* tmp = nullptr;
    //   s->next = nullptr;
    //   while (q != nullptr) {
    //     tmp = s;
    //     s = q;
    //     q = q->next;
    //     s->next = tmp;
    //   }
    //   head = s;
    //   return head;
    // }

    // recursive
    ListNode* reverseList(ListNode* head) {
      if (head == nullptr) {
        return head;
      }
      ListNode* s = head;
      ListNode* q = head->next;
      head->next = nullptr;
      return recursiveReverseList(s, q);
    }
    ListNode* recursiveReverseList(ListNode* s, ListNode* q) {
      if (q == nullptr) {
        return s;
      }
      ListNode* temp = s;
      s = q;
      q = q->next;
      s->next = temp;
      return recursiveReverseList(s, q);
    }

};
// @lc code=end

int main(int argc, char const *argv[])
{
    Solution s;
    ListNode*head = new ListNode(1, new ListNode(2, new ListNode(3, new ListNode(4, new ListNode(5)))));
    ListNode* new_head = s.reverseList(head);
    while (new_head != nullptr) {
      cout << new_head->val << endl;
      new_head = new_head->next;
    }
    return 0;
}
