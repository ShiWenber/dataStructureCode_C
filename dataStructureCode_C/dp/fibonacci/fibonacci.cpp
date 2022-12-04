/**
 * @file fibonacci.cpp
 * @author your name (you@domain.com)
 * @brief 动态规划斐波那契数列和递归法斐波那契数列的对比
 * @version 0.1
 * @date 2022-11-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <chrono>
#include <iostream>

using namespace std;
using namespace chrono;

int F(int, int*);
int fib_recursive(int n) {
  if (n == 1 || n == 2) {
    return 1;
  }
  return fib_recursive(n - 1) + fib_recursive(n - 2);
}

int fib_dp(int n) {
  /**分配空间并初始化为0*/
  int *dp = (int *)calloc(n + 1, sizeof(int));
  /**初始条件*/
  dp[1] = 1;
  dp[2] = 1;
  /**状态转移方程用函数封装了，方便递归*/
  return F(n, dp);
}

int F(int n, int *dp) {
  if (dp[n] != 0) {
    return dp[n];
  }
  dp[n] = F(n - 1, dp) + F(n - 2, dp);
  return dp[n];
}

int main() {
  int a;
  int b;
  int n;
  cin >> n;

  system_clock::time_point start2 = system_clock::now();
  b = fib_dp(n);
  system_clock::time_point end2 = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end2 - start2).count() / 1e6
       << "s" << endl;
  cout << "b=" << b << endl;

  system_clock::time_point start1 = system_clock::now();
  a = fib_recursive(n);
  system_clock::time_point end1 = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end1 - start1).count() / 1e6
       << "s" << endl;
  cout << "a=" << a << endl;

  return 0;
}