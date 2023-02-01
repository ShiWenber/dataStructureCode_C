/*
 * @lc app=leetcode.cn id=53 lang=cpp
 *
 * [53] 最大子数组和
 *
 * https://leetcode.cn/problems/maximum-subarray/description/
 *
 * algorithms
 * Medium (54.86%)
 * Likes:    5414
 * Dislikes: 0
 * Total Accepted:    1.2M
 * Total Submissions: 2.2M
 * Testcase Example:  '[-2,1,-3,4,-1,2,1,-5,4]'
 *
 * 给你一个整数数组 nums
 * ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。
 *
 * 子数组 是数组中的一个连续部分。
 *
 *
 *
 * 示例 1：
 *
 *
 * 输入：nums = [-2,1,-3,4,-1,2,1,-5,4]
 * 输出：6
 * 解释：连续子数组 [4,-1,2,1] 的和最大，为 6 。
 *
 *
 * 示例 2：
 *
 *
 * 输入：nums = [1]
 * 输出：1
 *
 *
 * 示例 3：
 *
 *
 * 输入：nums = [5,4,-1,7,8]
 * 输出：23
 *
 *
 *
 *
 * 提示：
 *
 *
 * 1 <= nums.length <= 10^5
 * -10^4 <= nums[i] <= 10^4
 *
 *
 *
 *
 * 进阶：如果你已经实现复杂度为 O(n) 的解法，尝试使用更为精妙的 分治法 求解。
 *
 */

#include <iostream>
#include <vector>
using namespace std;

// @lc code=start
class Solution {
 public:
  int maxSubArray(vector<int>& nums) {
    int head = 0;
    int tail = 0;
    return maxSubArray_divided(nums, 0, nums.size() - 1, head, tail);
  }
  int maxSubArray_divided(vector<int>& nums, int head, int tail, int& max_head,
                          int& max_tail) {
    int len = tail - head + 1;
    /**边界条件*/
    if (len == 1) {
      max_head = head;
      max_tail = tail;
      return nums[head];
    }
    int max_head_l = max_head;
    int max_tail_l = max_tail;

    int max_head_r = max_head;
    int max_tail_r = max_tail;
    /**分治*/
    int max_l = maxSubArray_divided(nums, head, (head + tail) / 2, max_head_l,
                                    max_tail_l);
    int max_r = maxSubArray_divided(nums, (head + tail) / 2 + 1, tail,
                                    max_head_r, max_tail_r);
    /**合并*/
    int max = 0;
    if (max_l > max_r) {
      max = max_l;
      max_head = max_head_l;
      max_tail = max_tail_l;
    } else {
      max = max_r;
      max_head = max_head_r;
      max_tail = max_tail_r;
    }
    /**找到横跨左右半区的子数组的最大和*/
    int mid = (head + tail) / 2;
    int sum_l = 0;
    int sum_l_max =
        0;  //<如果不选,最大和就是0
    int sum_r = 0;
    int sum_r_max =
        0;  //<可能出现子数组全为负数的情况，如果中间无法向右延申即为0
    /**向左延申*/
    int l_m = mid;
    for (int l = mid - 1; l >= head; l--) {
      sum_l += nums[l];
      if (sum_l > sum_l_max) {
        sum_l_max = sum_l;
        l_m = l;
      }
    }
    /**向右延申*/
    int r_m = mid;
    for (int r = mid + 1; r <= tail; r++) {
      sum_r += nums[r];
      if (sum_r > sum_r_max) {
        sum_r_max = sum_r;
        r_m = r;
      }
    }
    int sum_m = sum_l_max + nums[mid] + sum_r_max;
    if (sum_m > max) {
      max = sum_m;
      max_head = l_m;
      max_tail = r_m;
    }
    return max;
  }
};

// @lc code=end
int main() {
  vector<int> nums = {8, -19, 5, -4, 20};
  Solution s;
  int max = s.maxSubArray(nums);
  cout << max << endl;
  return 0;
}