/**
 * @file insert_sort_r.cpp
 * @author your name (you@domain.com)
 * @brief 递归实现的插入排序 
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
 * @brief 递归实现的插入排序,每次将一个元素插入到已经排好序的数组中，子问题是将数组的前n-1个元素排序
 * 
 * @param arr 输入数组
 * @param len 输入数组的长度
 * @return int 返回函数执行状态
 */
int insert_sort_r(double *arr, int len) {
    if (arr == NULL || len <= 0) {
        cout << "array is empty" << endl;
        return -1;
    }
    /**边界条件，当数组长度为1时不需要排序，递归截至*/
    if (len == 1) {
        return 0;
    }
    /**先将前面的部分排序*/
    insert_sort_r(arr, len - 1);
    /**暂存需要插入的数,由于递归，认为前面的数都是有序的*/
    double to_insert = arr[len - 1];
    /**用来指示当前位置*/
    int j = len - 2;
    /**从后往前与待插入数比较*/
    while(j >= 0 && arr[j] > to_insert) {
        /**如果当前数大于待插入数，则将当前数后移一位*/
        arr[j + 1] = arr[j];
        j--;
    }
    arr[j + 1] = to_insert;
    return 0; 
}

int main() {
    double arr[10000];
    random_array(arr, 10000);
    system_clock::time_point start = system_clock::now();
    insert_sort_r(arr, sizeof(arr)/sizeof(double));
    system_clock::time_point end = system_clock::now();
    for (int i = 0; i < 10000; i++) {
        cout << arr[i] << " ";
    }
    cout << endl << "time: " << duration_cast<microseconds>(end - start).count() / 1e6 << "s" << endl;
    return 0;
}