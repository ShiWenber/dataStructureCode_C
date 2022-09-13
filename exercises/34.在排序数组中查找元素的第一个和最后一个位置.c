/*
 * @lc app=leetcode.cn id=34 lang=c
 *
 * [34] 在排序数组中查找元素的第一个和最后一个位置
 *
 * https://leetcode-cn.com/problems/find-first-and-last-position-of-element-in-sorted-array/description/
 *
 * algorithms
 * Medium (42.32%)
 * Likes:    1443
 * Dislikes: 0
 * Total Accepted:    428K
 * Total Submissions: 1M
 * Testcase Example:  '[5,7,7,8,8,10]\n8'
 *
 * 给定一个按照升序排列的整数数组 nums，和一个目标值 target。找出给定目标值在数组中的开始位置和结束位置。
 * 
 * 如果数组中不存在目标值 target，返回 [-1, -1]。
 * 
 * 进阶：
 * 
 * 
 * 你可以设计并实现时间复杂度为 O(log n) 的算法解决此问题吗？
 * 
 * 
 * 
 * 
 * 示例 1：
 * 
 * 
 * 输入：nums = [5,7,7,8,8,10], target = 8
 * 输出：[3,4]
 * 
 * 示例 2：
 * 
 * 
 * 输入：nums = [5,7,7,8,8,10], target = 6
 * 输出：[-1,-1]
 * 
 * 示例 3：
 * 
 * 
 * 输入：nums = [], target = 0
 * 输出：[-1,-1]
 * 
 * 
 * 
 * 提示：
 * 
 * 
 * 0 
 * -10^9 
 * nums 是一个非递减数组
 * -10^9 
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>

// @lc code=start


/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
//返回第一个目标的下标
int searchLeft(int * nums, int numsSize, int target){
    int left = 0, right = numsSize - 1;
    int middle = 0;
    while(left < right){
        middle = (left + right) / 2;
        if(nums[middle] == target){
            right = middle; // 因为这个target有可能就是最左边的值，所以右边界不能减1,由于没有加减1也可能导致死循环
        }else if(target > nums[middle]){
            left = middle + 1;
            // key = 1;
        }else{
            right = middle - 1;
            // key = 0;
        }
    }
    //易错点 left == right,在向下取整时是一定满足的，但是如果是向上取整，可能出现left 比 right 大1的情况，还需在下面的条件中写明
    if(left == right && nums[left] == target){
        return left;
    }else{
        return -1;
    }
}

int searchRight(int * nums, int numsSize, int target){
    int left = 0, right = numsSize - 1;
    int middle = 0;
    while(left < right){
        middle = (left + right + 1) / 2; // 易错点：找左边开始位置要下取整，找右边开始位置要“上取整”(就是加1后再/2)
        if(nums[middle] == target){
            left = middle; // 因为这个target有可能就是最左边的值，所以右边界不能减1
        }else if(target > nums[middle]){
            left = middle + 1;
            // key = 1;
        }else{
            right = middle - 1;
            // key = 0;
        }
    }
    //易错点 left == right,在向下取整时是一定满足的，但是如果是向上取整，可能出现left 比 right 大1的情况，还需在下面的条件中写明
    if(left == right && nums[left] == target){
        return left;
    }else{
        return -1;
    }
}
int * searchRange(int* nums, int numsSize, int target, int* returnSize){
    *returnSize = 2;
    int *ans = (int*)malloc(2*sizeof(int));
    if(numsSize == 0){
        ans[0] = -1;
        ans[1] = -1;
        return ans;
    }else{
        ans[0] = searchLeft(nums, numsSize, target);
        ans[1] = searchRight(nums, numsSize, target);
        return ans;
    }
}
// @lc code=end
int main(){
    int nums[] = {2,2};
    int target = 3;
    int * returnSize = (int*)malloc(1*sizeof(int));
    int * ans = searchRange(nums, 2, target, returnSize);
    printf("[%d,%d]\n", ans[0], ans[1]);
    return 0;
}
