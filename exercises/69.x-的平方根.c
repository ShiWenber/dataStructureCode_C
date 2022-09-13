/*
 * @lc app=leetcode.cn id=69 lang=c
 *
 * [69] x 的平方根 
 *
 * https://leetcode-cn.com/problems/sqrtx/description/
 *
 * algorithms
 * Easy (39.03%)
 * Likes:    882
 * Dislikes: 0
 * Total Accepted:    452.5K
 * Total Submissions: 1.2M
 * Testcase Example:  '4'
 *
 * 给你一个非负整数 x ，计算并返回 x 的 算术平方根 。
 * 
 * 由于返回类型是整数，结果只保留 整数部分 ，小数部分将被 舍去 。
 * 
 * 注意：不允许使用任何内置指数函数和算符，例如 pow(x, 0.5) 或者 x ** 0.5 。
 * 
 * 
 * 
 * 示例 1：
 * 
 * 
 * 输入：x = 4
 * 输出：2
 * 
 * 
 * 示例 2：
 * 
 * 
 * 输入：x = 8
 * 输出：2
 * 解释：8 的算术平方根是 2.82842..., 由于返回类型是整数，小数部分将被舍去。
 * 
 * 
 * 
 * 
 * 提示：
 * 
 * 
 * 0 <= x <= 2^31 - 1
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>

// @lc code=start

//和二分法插入很相似
int mySqrt(int x){
    int left = 0, right = x;
    double mid = 0; // mid要进行2^31次方级别的数平方，可能导致溢出
    double temp = 0;
    int key = 0;
    while(left <= right){
        mid = (left + right) / 2;
        if((temp = mid * mid) == x){
            return mid;
        }else if(temp > x){
            right = mid - 1;
            key = -1;
        }else{
            left = mid + 1;
            key = 0;
        }
    }
    return mid + key;
}
// @lc code=end
int main(){
    printf("%d",mySqrt(2147395599));
    return 0;
}

