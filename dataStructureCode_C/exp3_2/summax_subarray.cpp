/**
 * @file summax_subarray.cpp
 * @author your name (you@domain.com)
 * @brief
 * 给定一个含有负数的数组，求和最大的非空连续子数组：也即求它的一个元素连续的子数组，该子数组元素的和为所有连续子数组中最大。如A={1,-4,3,-1，2,-3}，则和最大的连续子数组为A[3...5]={3,-1，2}
 * @version 0.1
 * @date 2022-11-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <chrono>
#include <iostream>
#include <limits>

#include "random_array.h"

using namespace std;
using namespace chrono;

/**
 * @brief 蛮力法求解最大子数据和
 *
 * @param arr 输入数组
 * @param len 数组长度
 * @param head 和最大的数组的头下标
 * @param tail 和最大的数组的尾下标
 * @return int 最大子数组的和值
 */
int summax_subarray_brute_force(double *arr, int len, int &head, int &tail) {
  if (arr == NULL || len <= 0) {
    cout << "input error" << endl;
    return numeric_limits<int>::min();
  }
  head = 0;
  tail = 0;
  int max = arr[0];
  for (int i = 0; i < len; i++) {
    int sum = 0;
    for (int j = i; j < len; j++) {
      sum += arr[j];
      if (sum > max) {
        max = sum;
        head = i;
        tail = j;
      }
    }
  }
  return max;
}

/**
 * @brief 分治法求解最大子数组和 
 * 
 * @param arr 输入数组 
 * @param head 输入数组的头下标
 * @param tail 输入数组的尾下标
 * @param max_head 最大子数组的头下标
 * @param max_tail 最大子数组的尾下标
 * @return int 最大子数组的和值
 */
int summax_subarray_divided(double *arr, int head, int tail, int &max_head,
                            int &max_tail) {
  int len = tail - head + 1;
  if (arr == NULL || len <= 0) {
    cout << "input error" << endl;
    return numeric_limits<int>::min();
  }
  /**边界条件*/
  if (len == 1) {
    max_head = head;
    max_tail = tail;
    return arr[head];
  }
  int max_head_l = max_head;
  int max_tail_l = max_tail;

  int max_head_r = max_head;
  int max_tail_r = max_tail;
  /**分治*/
  int max_l = summax_subarray_divided(arr, head, (head + tail) / 2, max_head_l,
                                      max_tail_l);
  int max_r = summax_subarray_divided(arr, (head + tail) / 2 + 1, tail,
                                      max_head_r, max_tail_r);
  /**合并*/
  int max = 0;
  if (max_l > max_r) {
    max = max_l;
    max_head = max_head_l;
    max_tail = max_tail_l;
  } else {
    max = max_r;
    max_head = max_head_r;
    max_tail = max_tail_r;
  }
  /**找到横跨左右半区的子数组的最大和*/
  int mid = (head + tail) / 2;
  int sum_l = 0;
  int sum_l_max = 0;  //<如果不选,最大和就是0
  int sum_r = 0;
  int sum_r_max = 0;  //<可能出现子数组全为负数的情况，如果中间无法向右延申即为0
  /**向左延申*/
  int l_m = mid;
  for (int l = mid - 1; l >= head; l--) {
    sum_l += arr[l];
    if (sum_l > sum_l_max) {
      sum_l_max = sum_l;
      l_m = l;
    }
  }
  /**向右延申*/
  int r_m = mid;
  for (int r = mid + 1; r <= tail; r++) {
    sum_r += arr[r];
    if (sum_r > sum_r_max) {
      sum_r_max = sum_r;
      r_m = r;
    }
  }
  int sum_m = sum_l_max + arr[mid] + sum_r_max;
  if (sum_m > max) {
    max = sum_m;
    max_head = l_m;
    max_tail = r_m;
  }
  return max;
}

int main() {
  double arr[] = {13,-3,-25,20,-3,-16,-23,18,20,-7,12,-5,-22,15,-4,7};
  int len = sizeof(arr) / sizeof(arr[0]);
  int head = 0;
  int tail = 0;
  int max = summax_subarray_divided(arr, 0, len - 1, head, tail);
  cout << "max:" << max << endl;
  cout << "head:" << head << endl;
  cout << "tail:" << tail << endl;
  for (int i = head; i <= tail; i++) {
    cout << arr[i] << " ";
  }

  // int len = 10000;
  // double arr[len];
  // int head = 0;
  // int tail = 0;
  // int head2 = 0;
  // int tail2 = 0;
  // int equ_times = 0;
  // srand((unsigned)time(NULL));
  // for (int i = 0; i < 100; i++) {
  //   for (int i = 0; i < len; i++) {
  //     /**生成从-5000到4999的随机数*/
  //     arr[i] = rand() % 10000 - 5000;
  //   }
  //   system_clock::time_point start = system_clock::now();
  //   int max = summax_subarray_brute_force(arr, len, head, tail);
  //   system_clock::time_point end = system_clock::now();
  //   cout << "time: " << duration_cast<microseconds>(end - start).count() / 1e6
  //        << "s" << endl;
  //   cout << "max = " << max << endl;
  //   cout << "head = " << head << endl;
  //   cout << "tail = " << tail << endl;
  //   cout << "----------------" << endl;
  //   system_clock::time_point start2 = system_clock::now();
  //   int max2 = summax_subarray_divided(arr, 0, len - 1, head2, tail2);
  //   system_clock::time_point end2 = system_clock::now();
  //   cout << "time: " << duration_cast<microseconds>(end2 - start2).count() / 1e6
  //        << "s" << endl;
  //   cout << "max = " << max2 << endl;
  //   cout << "head = " << head2 << endl;
  //   cout << "tail = " << tail2 << endl;
  //   cout << endl;
  //   if (max == max2 && head == head2 && tail == tail2) {
  //     equ_times++;
  //   }
  // }
  // cout << endl;
  // cout << "equ_times = " << equ_times << endl;

  return 0;
}