/*
 * @lc app=leetcode.cn id=704 lang=c
 *
 * [704] 二分查找
 */

#include <stdio.h>
#include <string.h>

// @lc code=start


int search(int* nums, int numsSize, int target){
    int left = 0;
    int right = numsSize - 1;
    int middle = 0;
    //左闭右开或闭区间，开侧不减一，闭侧加减一,引入开区间不保险，如果只有一个元素会出错
    while(left <= right){
        middle = (left + right) / 2;
        if(nums[middle] == target){
            return middle;
        }else if(target < nums[middle]){
            right = middle - 1;
        }else{
            left = middle + 1;
        }
    }
    return -1;
}

// @lc code=end

//测试用主函数
int main(){
    int nums[] = {1,2,3,4,5,6,7,8,9,10};
    int target = 5;
    printf("%d",search(nums,10,target));
    return 0;
}