/**
 * @file BubbleSort.cpp
 * @author your name (you@domain.com)
 * @brief 冒泡排序 
 * @version 0.1
 * @date 2022-10-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include "random_array.h"

using namespace std;

/**
 * @brief 冒泡排序, 默认升序排序 
 * 
 * @param a 待排序数组
 * @param size 数组长度
 * @param asc 是否升序排序，默认为true
 * @return int 返回函数执行状态，0为正常
 */
int bubble_sort(double *a, int size, bool asc = true) {
  if (a == NULL || size <= 0) {
    cout << "array is empty" << endl;
    return -1;
  }
  double temp = 0;
  if (asc == true) {
    for (int i = 0; i < size; i++)
    {
      for (int j = 0; j < size - i - 1; j++)
      {
        if (a[j] > a[j + 1]) {
          temp = a[j];
          a[j] = a[j + 1];
          a[j + 1] = temp;
        }
      }
    }
  } else {
    for (int i = 0; i < size; i++)
    {
      for (int j = 0; j < size - i - 1; j++)
      {
        if (a[j] < a[j + 1]) {
          temp = a[j];
          a[j] = a[j + 1];
          a[j + 1] = temp;
        }
      }
    }
  }
  return 0;
}

int main() {
  double a[100000];
  /**自定义的生成随机数的代码*/
  random_array(a, 100000);
  /**记录开始时间*/ 
  clock_t start = clock();
  cout << "start: " << endl;
  bubble_sort(a, sizeof(a)/sizeof(double));
  /**结束时间*/
  clock_t end = clock();
  cout << endl << "time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
  /**输出排序结果*/
  for (int i = 0; i < sizeof(a)/sizeof(double); i++)
  {
    cout << a[i] << " ";
  }
  cout << endl << "time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
  return 0;
}