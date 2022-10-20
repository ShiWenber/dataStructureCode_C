/**
 * @file selection_sort_r.cpp
 * @author your name (you@domain.com)
 * @brief 递归的选择排序
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
 * @brief 递归实现的选择排序，没次都将最大的放到最后，子问题是前n-1个元素排序 
 * 
 * @param arr 输入数组 
 * @param len 输入数组长度
 * @return int 返回函数执行状态
 */
int selection_sort_r(double *arr, int len) {
    if (arr == NULL || len <= 0) {
        cout << "array is empty" << endl;
        return 0;
    }
    if (len == 1) {
        return 0;
    }
    /**每次将最大的值交换到末尾*/
    int max = len - 1;
    for (int i = 0; i < len - 1; i++) {
        if (arr[i] > arr[max]) {
            max = i;
        }
    }
    if (max != len - 1) {
        double temp = arr[max];
        arr[max] = arr[len - 1];
        arr[len - 1] = temp;
    }
    selection_sort_r(arr, len - 1);
    return 0;
}

int main() {
    double arr[10000];
    random_array(arr, 10000);

    cout << "start: " << endl;
    system_clock::time_point start = system_clock::now();
    selection_sort_r(arr, sizeof(arr)/sizeof(double));
    system_clock::time_point end = system_clock::now();
    /**输出结果*/
    for (int i = 0; i < 10000; i++) {
        cout << arr[i] << " ";
    }
    cout << endl << "time:" << duration_cast<microseconds>(end - start).count() / 1e6 << "s" << endl;

}



