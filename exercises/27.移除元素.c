/*
 * @lc app=leetcode.cn id=27 lang=c
 *
 * [27] 移除元素
 *
 * https://leetcode-cn.com/problems/remove-element/description/
 *
 * algorithms
 * Easy (59.57%)
 * Likes:    1172
 * Dislikes: 0
 * Total Accepted:    580.4K
 * Total Submissions: 974.4K
 * Testcase Example:  '[3,2,2,3]\n3'
 *
 * 给你一个数组 nums 和一个值 val，你需要 原地 移除所有数值等于 val 的元素，并返回移除后数组的新长度。
 * 
 * 不要使用额外的数组空间，你必须仅使用 O(1) 额外空间并 原地 修改输入数组。
 * 
 * 元素的顺序可以改变。你不需要考虑数组中超出新长度后面的元素。
 * 
 * 
 * 
 * 说明:
 * 
 * 为什么返回数值是整数，但输出的答案是数组呢?
 * 
 * 请注意，输入数组是以「引用」方式传递的，这意味着在函数里修改输入数组对于调用者是可见的。
 * 
 * 你可以想象内部操作如下:
 * 
 * 
 * // nums 是以“引用”方式传递的。也就是说，不对实参作任何拷贝
 * int len = removeElement(nums, val);
 * 
 * // 在函数里修改输入数组对于调用者是可见的。
 * // 根据你的函数返回的长度, 它会打印出数组中 该长度范围内 的所有元素。
 * for (int i = 0; i < len; i++) {
 * print(nums[i]);
 * }
 * 
 * 
 * 
 * 
 * 示例 1：
 * 
 * 
 * 输入：nums = [3,2,2,3], val = 3
 * 输出：2, nums = [2,2]
 * 解释：函数应该返回新的长度 2, 并且 nums 中的前两个元素均为 2。你不需要考虑数组中超出新长度后面的元素。例如，函数返回的新长度为 2 ，而
 * nums = [2,2,3,3] 或 nums = [2,2,0,0]，也会被视作正确答案。
 * 
 * 
 * 示例 2：
 * 
 * 
 * 输入：nums = [0,1,2,2,3,0,4,2], val = 2
 * 输出：5, nums = [0,1,4,0,3]
 * 解释：函数应该返回新的长度 5, 并且 nums 中的前五个元素为 0, 1, 3, 0,
 * 4。注意这五个元素可为任意顺序。你不需要考虑数组中超出新长度后面的元素。
 * 
 * 
 * 
 * 
 * 提示：
 * 
 * 
 * 0 
 * 0 
 * 0 
 * 
 * 
 */
#include <stdio.h>

// @lc code=start
int removeElement(int* nums, int numsSize, int val){
    // // 法一，数组的一般删除操作, 暴力解法， 每次删除一个元素都更新一次数组， 时间复杂度为 O(n^2)
    // for(int i = 0; i < numsSize; i++){
    //     if(nums[i] == val){
    //         // 找到val后将该元素删除并且更新数组
    //         for(int j = i + 1; j < numsSize; j++){
    //             nums[j - 1] = nums[j]; // 建议用j = i + 1, 结束标志为< numsSize, 防止越界
    //         }
    //         numsSize--;
    //         i--; // 注意删除后将i复位
    //     }
    // }
    // return numsSize;

     


    // //法二，更优化的算法, 不能使用队列，因为本题对时间复杂度有要求, 双指针法， 复杂度为2n, 时间复杂度为 O(2n) = O(n)
    // int i = 0; // 指针1
    // int j = 0; // 指针2
    // int temp = 0;
    // // 第一次给j定位（第一次给j定位到i后面，此后j变化再与i无关）
    // for(;i < numsSize && nums[i] != val; i++);
    // if(i == numsSize)
    //     return numsSize; // 一个val都找不到
    // j = i + 1;
    // // j 定位完成，因为在嵌套循环中j与i值无关，所以不会导致n^2的复杂度
    // for(; i < numsSize; i++){
    //     if(nums[i] == val){
    //         // 找到第一个val后用i记录其位置， 并将j往后遍历直到第一个不是val的位置
    //         for(; j < numsSize && nums[j] == val; j++);
    //         // 根据德摩根律， 此时 j >= numsSize || nums[j] != val, 由于j达到条件立刻退出， 所以j = numsSize || nums[j] != val， 而j = numsSize 必然会引起越界
    //         if(j == numsSize){
    //             // 如果j达到末尾，说明数组中i以及i之后没有其他的val， 则直接返回
    //             return i; // 从0 ~ i 的 i - 1个元素都不是val, 其他的元素都是val
    //         }else{
    //             // j != numsSize, 由上述讨论, 到此处nums[j] != val
    //             temp = nums[i];
    //             nums[i] = nums[j];
    //             nums[j] = temp;
    //             // 删除2次
    //         }
    //     }
    // }
    // return numsSize;
    
    
    // （最有选择）法三: 双指针法,  双指针的距离差表示了删除元素的个数， 每次碰到val就让fastIndex++, 同时slowIndex不变，如果fastIndex指向的元素不是val，则slowIndex没碰到val, 两者同步变化， 不会增大距离， 并将fastIndex内容给slowIndex， 相当于将两指针中间的内容省略掉了
    int slowIndex = 0;
    for (int fastIndex = 0; fastIndex < numsSize; fastIndex++) {
        if (nums[fastIndex] != val) {
            nums[slowIndex++] = nums[fastIndex];
        }
    }
    // slowIndex 表示数组的最终长度， 因为slowIndex和fastIndex 之间可以认为是删去的元素
    return slowIndex;
}
// @lc code=end

int main(){
    int nums[] = {0, 1, 2, 2, 3, 0, 4, 2};
    int numsSize = sizeof(nums) / sizeof(int);
    int val = 2;
    int size = removeElement(nums, numsSize, val);
    for(int i = 0; i < size; i++){
        printf("%d ", nums[i]);
    }
}