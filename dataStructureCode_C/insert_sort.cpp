/**
 * @file insert_sort.cpp
 * @author your name (you@domain.com)
 * @brief 插入排序 
 * @version 0.1
 * @date 2022-10-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include "random_array.h"

using namespace std;

/**
 * @brief 插入排序, 默认升序排序 
 * 
 * @param a 待排序数组
 * @param size 数组长度
 * @param asc 是否升序排序，默认为true
 * @return int 返回函数执行状态，0为正常
 */
int insert_sort(double *a, int size, bool asc = true) {
  if (a == NULL || size <= 0) {
    cout << "array is empty" << endl;
    return -1;
  }
  double temp = 0;
  if (asc == true) {
    /**空出数组第一个,保证初始情况下有位置交换*/
    for (int i = 1; i < size; i++)
    {
      temp = a[i];
      int j = i - 1;
      while (j >= 0 && a[j] > temp) {
        a[j + 1] = a[j];
        j--;
      }
      /**此时a[j] <= temp 或者 j == 0 或两者同时成立*/
      a[j + 1] = temp;
    }
  } else {
    /**空出数组第一个*/
    for (int i = 1; i < size; i++)
    {
      temp = a[i];
      int j = i - 1;
      while (j >= 0 && a[j] < temp) {
        a[j + 1] = a[j];
        j--;
      }
      a[j + 1] = temp;
    }
  }
  return 0;
}


int main() {
  double a[10000];
  /**自定义的生成随机数的代码*/
  random_array(a, 10000);
  /**记录开始时间*/ 
  clock_t start = clock();
  cout << "start: " << endl;
  insert_sort(a, sizeof(a)/sizeof(double));
  /**结束时间*/
  clock_t end = clock();
  cout << endl << "time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
  /**输出排序结果*/
  for(int i = 0; i < 10000; i++) {
    cout << a[i] << " ";
  }
  cout << endl << "time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
  return 0;
}