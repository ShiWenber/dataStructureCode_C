/**
 * @file select_sort.cpp
 * @author your name (you@domain.com)
 * @brief 选择排序 
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
 * @brief 选择排序, 默认升序排序 
 * 
 * @param a 待排序数组
 * @param size 数组长度
 * @param asc 是否升序排序，默认为true
 * @return int 返回函数执行状态，0为正常
 */
int selection_sort(double *a, int size, bool asc = true) {
  if (a == NULL || size <= 0) {
    cout << "array is empty" << endl;
    return -1;
  }
  double temp = 0;
  if (asc == true) {
    /**课件伪代码中循环都是从1开始，因此这里需要修改*/
    for (int i = 0; i < size - 1; i++)
    {
      int min = i;
      for (int j = i + 1; j < size; j++)
      {
        if (a[j] < a[min]) {
          min = j;
        }
      }
      /**这里加不加条件判断都对*/
      if (min != i) {
        temp = a[i];
        a[i] = a[min];
        a[min] = temp;
      }
      // temp = a[i];
      // a[i] = a[min];
      // a[min] = temp;
    }
  } else {
    for (int i = 0; i < size - 1; i++)
    {
      int max = i;
      for (int j = i + 1; j < size; j++)
      {
        if (a[j] > a[max]) {
          max = j;
        }
      }
      temp = a[i];
      a[i] = a[max];
      a[max] = temp;
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
  selection_sort(a, sizeof(a)/sizeof(double));
  /**结束时间*/
  clock_t end = clock();
  cout << endl << "time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
  /**输出排序结果和用时*/
  for (int i = 0; i < sizeof(a)/sizeof(double); i++)
  {
    cout << a[i] << " ";
  }
  cout << endl << "time: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
}
