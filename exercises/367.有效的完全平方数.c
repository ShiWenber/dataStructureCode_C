/*
 * @lc app=leetcode.cn id=367 lang=c
 *
 * [367] 有效的完全平方数
 *
 * https://leetcode-cn.com/problems/valid-perfect-square/description/
 *
 * algorithms
 * Easy (44.83%)
 * Likes:    347
 * Dislikes: 0
 * Total Accepted:    129.3K
 * Total Submissions: 288.5K
 * Testcase Example:  '16'
 *
 * 给定一个 正整数 num ，编写一个函数，如果 num 是一个完全平方数，则返回 true ，否则返回 false 。
 * 
 * 进阶：不要 使用任何内置的库函数，如  sqrt 。
 * 
 * 
 * 
 * 示例 1：
 * 
 * 
 * 输入：num = 16
 * 输出：true
 * 
 * 
 * 示例 2：
 * 
 * 
 * 输入：num = 14
 * 输出：false
 * 
 * 
 * 
 * 
 * 提示：
 * 
 * 
 * 1 
 * 
 * 
 */
#include <stdbool.h>
#include <stdio.h>


// @lc code=start


bool isPerfectSquare(int num){
    int left = 0, right = num;
    double mid = 0, temp = 0;
    while(left < right){
        mid = (left + right) / 2;
        if((temp = mid * mid) == num){
            return true;
        }else if(temp > num){
            right = mid - 1;
        }else{
            left = mid + 1;
        }
    }
    if(left == right && left * left == num){
        return true;
    }else{
        return false;
    }
}
// @lc code=end
int main(){
    printf("%d\n", isPerfectSquare(14));
    return 0;
}
