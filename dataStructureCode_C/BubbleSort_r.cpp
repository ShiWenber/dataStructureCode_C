/**
 * @file BubbleSort_r.cpp
 * @author your name (you@domain.com)
 * @brief 冒泡排序的递归实现 
 * @version 0.1
 * @date 2022-10-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <chrono>
#include "random_array.h"


using namespace std;
using namespace chrono;


/**
 * @brief 冒泡排序递归实现，每次将最大的元素放到最后，子问题为从未排序的元素中找出最大的元素放到最后
 * 
 * @param arr 输入数组
 * @param len 数组长度
 */
void bubble_sort_r(double arr[], int len) {
  if (len <= 1) {
    return;
  }
  for (int i = 0; i < len - 1; i++) {
    if (arr[i] > arr[i + 1]) {
      int temp = arr[i];
      arr[i] = arr[i + 1];
      arr[i + 1] = temp;
    }
  }
  bubble_sort_r(arr, len - 1);
}

int main() {
    double arr[10000];
    random_array(arr, 10000);
    cout << "start: " << endl;
    system_clock::time_point start = system_clock::now();
    bubble_sort_r(arr, sizeof(arr)/sizeof(double));
    system_clock::time_point end = system_clock::now();
    // 输出运行时间,单位为s
    for (int i = 0; i < 10000; i++) {
      cout << arr[i] << " ";
    }
    cout << endl << "time:" << duration_cast<microseconds>(end - start).count() / 1e6 << "s" << endl;
    return 0;
}

    
    