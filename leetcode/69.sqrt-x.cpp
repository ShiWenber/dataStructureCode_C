/*
 * @lc app=leetcode.cn id=69 lang=cpp
 *
 * [69] Sqrt(x)
 *
 * https://leetcode.cn/problems/sqrtx/description/
 *
 * algorithms
 * Easy (38.48%)
 * Likes:    1506
 * Dislikes: 0
 * Total Accepted:    845.8K
 * Total Submissions: 2.2M
 * Testcase Example:  '4'
 *
 * Given a non-negative integer x, return the square root of x rounded down to
 * the nearest integer. The returned integer should be non-negative as well.
 * 
 * You must not use any built-in exponent function or operator.
 * 
 * 
 * For example, do not use pow(x, 0.5) in c++ or x ** 0.5 in python.
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: x = 4
 * Output: 2
 * Explanation: The square root of 4 is 2, so we return 2.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: x = 8
 * Output: 2
 * Explanation: The square root of 8 is 2.82842..., and since we round it down
 * to the nearest integer, 2 is returned.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= x <= 2^31 - 1
 * 
 * 
 */
#include <iostream>
using namespace std;

// @lc code=start
class Solution {
public:
    int mySqrt(int x) {
        int l = 1;     
        int r = x;
        double m = 0;
        while (l <= r) {
            m = l + (r - l) / 2;
            if (m*m == x) {
                return m;
            } else if (m*m < x) {
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        // l > r 因此返回小的是下届
        return r;
    }
};
// @lc code=end

int main() {
  Solution s;
  int x = 2147395599;
  int y = 46340;
  cout << s.mySqrt(x) << endl;
  cout << y * y << endl;
  return 0;
}