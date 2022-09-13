/*
 * @lc app=leetcode.cn id=35 lang=c
 *
 * [35] 搜索插入位置
 *
 * https://leetcode-cn.com/problems/search-insert-position/description/
 *
 * algorithms
 * Easy (45.72%)
 * Likes:    1334
 * Dislikes: 0
 * Total Accepted:    650K
 * Total Submissions: 1.4M
 * Testcase Example:  '[1,3,5,6]\n5'
 *
 * 给定一个排序数组和一个目标值，在数组中找到目标值，并返回其索引。如果目标值不存在于数组中，返回它将会被按顺序插入的位置。
 * 
 * 请必须使用时间复杂度为 O(log n) 的算法。
 * 
 * 
 * 
 * 示例 1:
 * 
 * 
 * 输入: nums = [1,3,5,6], target = 5
 * 输出: 2
 * 
 * 
 * 示例 2:
 * 
 * 
 * 输入: nums = [1,3,5,6], target = 2
 * 输出: 1
 * 
 * 
 * 示例 3:
 * 
 * 
 * 输入: nums = [1,3,5,6], target = 7
 * 输出: 4
 * 
 * 
 * 示例 4:
 * 
 * 
 * 输入: nums = [1,3,5,6], target = 0
 * 输出: 0
 * 
 * 
 * 示例 5:
 * 
 * 
 * 输入: nums = [1], target = 0
 * 输出: 0
 * 
 * 
 * 
 * 
 * 提示:
 * 
 * 
 * 1 
 * -10^4 
 * nums 为无重复元素的升序排列数组
 * -10^4 
 * 
 * 
 */
#include <stdio.h>

// @lc code=start

//用二分法为有序序列插入值，存在相等值的不做处理
int searchInsert(int* nums, int numsSize, int target){
    int left = 0;
    int right = numsSize - 1;
    int middle = 0;
    int key = 0; // 标记while循环从何种情况退出，没找到退出时一定左边比右边大一，在完成加减一之前一定left == right == middle
    while(left <= right){
        middle = (left + right) / 2;
        if(target == nums[middle]){
            return middle;
        }else if(target > nums[middle]){
            left = middle + 1;
            key = 1;//大于中间值的情况退出循环则在middle加0后插入
        }else{//到此处一定target < nums[middle]
            right = middle - 1;
            key = 0;//小于中间值的情况退出循环则在middle减1后插入
        }
    }
    // 到此处表示没找到，key为1表示退出循环前target在mid位置的右边，key为0表示在mid位置的左边，当返回mid + key的时候（不管有没有找到），mid + key 就表示应该插入的位置
    return middle + key;
}
// @lc code=end
int main(){
    int nums[] = {1,3,5,6};
    int target = 7;
    printf("%d",searchInsert(nums,4,target));
    return 0;
}
