/*
 * @lc app=leetcode.cn id=977 lang=c
 *
 * [977] 有序数组的平方
 *
 * https://leetcode-cn.com/problems/squares-of-a-sorted-array/description/
 *
 * algorithms
 * Easy (69.56%)
 * Likes:    452
 * Dislikes: 0
 * Total Accepted:    269.3K
 * Total Submissions: 388.3K
 * Testcase Example:  '[-4,-1,0,3,10]'
 *
 * 给你一个按 非递减顺序 排序的整数数组 nums，返回 每个数字的平方 组成的新数组，要求也按 非递减顺序 排序。
 * 
 * 
 * 
 * 
 * 
 * 
 * 示例 1：
 * 
 * 
 * 输入：nums = [-4,-1,0,3,10]
 * 输出：[0,1,9,16,100]
 * 解释：平方后，数组变为 [16,1,0,9,100]
 * 排序后，数组变为 [0,1,9,16,100]
 * 
 * 示例 2：
 * 
 * 
 * 输入：nums = [-7,-3,2,3,11]
 * 输出：[4,9,9,49,121]
 * 
 * 
 * 
 * 
 * 提示：
 * 
 * 
 * 1 10^4
 * -10^4 
 * nums 已按 非递减顺序 排序
 * 
 * 
 * 
 * 
 * 进阶：
 * 
 * 
 * 请你设计时间复杂度为 O(n) 的算法解决本问题
 * 
 * 
 */

// @lc code=start


/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* sortedSquares(int* nums, int numsSize, int* returnSize){
    // 直观想法: 先遍历取出，再按排序形成新数组
    // 时间复杂度: O(n) = O(n + nlogn + n^2), n是遍历， nlogn是二分查找， n^2是二分查找后插入，
    // 法一：遍历: {平方 -- 快排}
    // （最优选择）法二：双指针法， 有点类似快排的双指针法， 从头向中间靠， 当指针靠拢时表示已经排序好了
    int left = 0;
    int right = numsSize - 1;
    int p = right; // 用来指向待插入位置的下标
    int * res = (int*)malloc(numsSize*sizeof(int)); 
    while (left <= right)
    {
        if (nums[left] * nums[left] < nums[right] * nums[right])
        {
            res[p--] = nums[right] * nums[right];
            right--;
        }else{
            res[p--] = nums[left] * nums[left];
            left++;
        }
    }
    *returnSize = numsSize; 
    return res;
}
// @lc code=end

