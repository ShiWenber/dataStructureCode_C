/**
 * @file hanoi.cpp
 * @author your name (you@domain.com)
 * @brief 递归汉诺塔
 * @version 0.1
 * @date 2022-10-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <chrono>
#include <math.h>


using namespace std;
using namespace chrono;
/**
 * @brief 递归实现的汉诺塔 
 * 
 * @param layer 汉诺塔层数 
 * @return int 移动次数
 */
int hanoi_r(int layer) {
  if (layer <= 0) {
    cout << "layer must be greater than 0" << endl;
    return 0;
  }
  /**边界条件*/
  if (layer == 1) {
    return 1;
  }
  return 2 * hanoi_r(layer - 1) + 1;
}

/**
 * @brief 非递归实现的汉诺塔，根据汉诺塔移动次数的递推公式计算得2^n-1
 * 
 * @param layer 汉诺塔层数
 * @return int 移动次数
 */
int hanoi(int layer) {
  if (layer <= 0) {
    cout << "layer must be greater than 0" << endl;
    return 0;
  }
  return pow(2.0, layer) - 1;
}

int main() {
  int layer = 0;
  cout << "layer: ";
  cin >> layer;
  /**记录开始时间*/
  system_clock::time_point start = system_clock::now();
  cout << "start: " << system_clock::to_time_t(start) << endl;
  double res = hanoi(layer);
  /**结束时间*/
  system_clock::time_point end = system_clock::now();
  cout << "res: " << res << endl;
  cout << "end: " << system_clock::to_time_t(end) << endl;
  cout << "time: " << duration_cast<microseconds>(end - start).count() << "us" << endl;
  return 0;
}
