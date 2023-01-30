/**
 * @file bottom_up_sort.cpp
 * @author your name (you@domain.com)
 * @brief 自底向上的合并排序
 * @version 0.1
 * @date 2022-10-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include "random_array.h"
#include <chrono>

using namespace chrono;
using namespace std;
/**
 * 
 * @brief 合并有序数组a[low1...high1]和a[low2...high2]，low2 = high1 + 1合并后的数组仍然有序， 
 * 
 * @param a 
 * @param low1 
 * @param high1 
 * @param high2 
 * @param size 数组a的长度
 * @return int 
 */
inline int merge(double *a, int low1, int high1, int high2, int size, double *temp) {
  /**s-high1为a1,t-high2为a2*/
  int s = low1, t = high1 + 1, k = low1; 
  while (s <= high1 && t <= high2) {
    /**将对应位置最小的值放入对应位置的temp中*/
    if (a[s] <= a[t]) {
      temp[k++] = a[s++];
    } else {
      temp[k++] = a[t++];
    }
  }
  /** s == high1 + 1 或者 t == high2 + 1*/
  if (s == (high1 + 1)) {
    while (t <= high2) {
      temp[k++] = a[t++];
    }
    /** t==high2 + 1*/
  } else {
    while (s <= high1) {
      temp[k++] = a[s++];
    }
  }
  while (low1 <= high2) {
    a[low1] = temp[low1];
    low1++;
  }
  return 0;
}
  
  

/**
 * @brief 归并排序，升序
 * 
 * @param a 待排序数组
 * @param size 数组长度
 * @return int 返回函数执行状态，0为正常
 */
inline int bottom_up_sort(double *a, int size) {
  if (a == NULL || size <= 0) {
    cout << "array is empty" << endl;
    return -1;
  }
  /** 临时数组，用于存放合并后的数组 */
  double *temp = new double[size]; // 用来减小创建数组的开销
  int step = 1; //< 合并子数组的长度
  // 终止条件，只要step大于size就表示排序已经完成
  while (step < size) {
    int var = step;
    step = 2 * var; // 二归并，每一步将step*2
    int i = 0;
    while(i + step + 1 < size) {
      merge(a, i, i + var - 1, i + step - 1, size, temp);
      i += step;
    }
    if (i + var < size) {
      merge(a, i, i + var - 1, size - 1, size, temp);
    }
  }
  return 0;
}
    

    
int main() {
  double a[100000];
  /**自定义的生成随机数的代码*/
  random_array(a, 100000);

  /**记录开始时间*/ 
  system_clock::time_point start = system_clock::now();
  bottom_up_sort(a, sizeof(a)/sizeof(double));
  /**结束时间*/
  system_clock::time_point end = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end - start).count() / 1e6 << "s" << endl;
  /**输出排序结果*/
  // for(int i = 0; i < sizeof(a)/ sizeof(double); i++) {
  //   cout << a[i] << " ";
  // }
  return 0;
}

