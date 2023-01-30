/**
 * @file bottom_up_sort_r.cpp
 * @author your name (you@domain.com)
 * @brief 递归实现的合并排序(自顶向下)
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
 * @brief 合并有序数组a[low1...high1]和a[high1+1...high2]，合并后的数组仍然有序，
 *
 * @param a
 * @param low1
 * @param high1
 * @param high2
 * @param size 数组a的长度
 * @param temp 临时数组，长度等于a的长度
 * @return int
 */
inline int merge(double *a, int low1, int high1, int high2, int size, double *temp)
{
  /**s-high1为a1,t-high2为a2*/
  int s = low1, t = high1 + 1, k = low1;
  while (s <= high1 && t <= high2)
  {
    /**将对应位置最小的值放入对应位置的temp中*/
    if (a[s] <= a[t])
    {
      temp[k++] = a[s++];
    }
    else
    {
      temp[k++] = a[t++];
    }
  }
  /** s == high1 + 1 或者 t == high2 + 1*/
  if (s == (high1 + 1))
  {
    while (t <= high2)
    {
      temp[k++] = a[t++];
    }
    /** t==high2 + 1*/
  }
  else
  {
    while (s <= high1)
    {
      temp[k++] = a[s++];
    }
  }
  /**将存储在temp临时变量中的值赋值回a中*/
  while (low1 <= high2)
  {
    a[low1] = temp[low1];
    low1++;
  }
  return 0;
}

/**
 * @brief 自顶向下合并排序，划分：等分两个子数组，治理：分别递归，全部在输入数组上直接操作就可以省去数组复制的资源消耗
 *
 * @param arr 输入数组
 * @param len 输入数组的长度
 * @param int 返回函数执行状态
 * @param temp 辅助数组，长度等于输入数组的长度
 */
int up_bottom_sort_r(double *arr, int l, int r, double *temp)
{
  int len = r - l + 1;
  // nullptr和NULL的区别
  if (arr == nullptr || len <= 0)
  {
    cout << "arr is nullptr or len <= 0" << endl;
    return -1;
  }
  /**边界条件*/
  if (len == 1)
  {
    return 0;
  }
  /**len不必区分奇偶*/
  /**划分*/
  up_bottom_sort_r(arr, l, l + len / 2 - 1, temp);
  up_bottom_sort_r(arr, l + len / 2, r, temp);
  /**治理，需要将两个子问题的结果统一为父问题的结果，这里用了指针数组，每次递归都是对该全局空间做修改的，所以传入merge的arr自然携带了上面求出的子问题的解*/
  merge(arr, l, l + len / 2 - 1, r, len, temp);
  return 0;
}

int main() {
  double arr[10000];
  random_array(arr, 10000);
  double temp[10000];
  system_clock::time_point start = system_clock::now();
  up_bottom_sort_r(arr, 0, 9999, temp);
  system_clock::time_point end = system_clock::now();
  for (int i = 0; i < 10000; i++)
  {
    cout << arr[i] << " ";
  }
  cout << endl << "time: " << duration_cast<microseconds>(end - start).count() / 1e6 << "s" << endl;
  return 0;
}
