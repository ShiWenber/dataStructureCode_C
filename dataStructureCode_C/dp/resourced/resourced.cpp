/**
 * @file resourced.cpp
 * @author your name (you@domain.com)
 * @brief
 * 将有限资源分配给若干个工程的问题：设资源总数是a，工程个数为n，给每项工程分配的资源数不同，获得的利润也不同。给定各工程的资源利润表和资源总数，应当怎么分配资源，才能获得最大的利润呢？
 * 有投资6万元，拟投产某工厂的A、B、C三种产品
 * Gi(xi)表示把资源xi分配给工程i的利润
 * x	0	1	2	3	4	5	6
 * Ga(x1)	0	1.2	1.5	1.85	2.4	2.8	3.3
 * Gb(x2)	0	1.8	2	2.25	2.4	2.5	2.6
 * Gc(x3)	0	1.3	1.9	2.2	2.45	2.7	3

 * @version 0.1
 * @date 2022-12-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "NdArray.h"

using namespace std;

template <typename T>
void read_csv(std::string filename, NdArray<T> &arr) {
  ifstream file(filename);
  string line;
  int i = 0;
  while (getline(file, line)) {
    stringstream ss(line);
    string cell;
    int j = 0;
    while (getline(ss, cell, ',')) {
      arr.assign(stod(cell), i, j);
      j++;
    }
    i++;
  }
}

/**
 * @brief 有限资源分配问题
 * 设g(i, j)表示将资源j分配给工程i的利润
 * 最终结果为 g(1, x1) + g(2, x2) + g(3, x3) + ... + g(n, xn)
 * 设f(i,j)表示给前i个工程投资j时的最大利润，递推公式为:
 * 三种情况:
 * f(1, j) = max{g(1, x1), x1=0,1,2,...,j}
 * f(2, j) = max{f(1, j-x2) + g(2, x2), x2=0,1,2,...,j}
 * f(3, j) = max{f(2, j-x3) + g(3, x3), x3=0,1,2,...,j}
 * ...
 * f(n, j) = max{f(n-1, j-xn) + g(n, xn), xn=0,1,2,...,j}
 *
 * @param arr 利润表
 * @param total 投资总额
 * @param plan 分配方案，plan(i, j)表示使f(i, j)最大时，给工程i分配的资源数
 * @return int 最大利润
 *
 *
 */
double max_profile(NdArray<double> &arr, int total, NdArray<double> &plan,
                   NdArray<double> &f) {
  int m = total;               //< 资源个数
  int n = arr.shape()[0] - 1;  //< 工程个数
  /** 为i = 1的情况赋初始值*/
  for (int i = 0; i <= m; i++) {
    f.assign(arr.at(2, 1, i), 1, i);
    plan.assign(i, 1, i);
  }
  /**遍历工程个数，为i!=1的情况赋值*/
  for (int i = 2; i <= n; i++) {
    f.assign(0, i, 0);
    plan.assign(0, i, 0);
    /**遍历查找最大f(i-1, j - k) + arr(i, k)*/
    for (int j = 1; j <= m; j++) {
      for (int k = 1; k <= j; k++) {
        if (f.at(2, i - 1, j - k) + arr.at(2, i, k) > f.at(2, i, j)) {
          f.assign(f.at(2, i - 1, j - k) + arr.at(2, i, k), i, j);
          plan.assign(k, i, j);
        }
      }
    }
  }

  return f.at(2, n, m);
}

int main() {
  NdArray<double> arr(0, 2, 4, 7);
  read_csv(
      "E:\\pros\\dataStructureCode_C\\dataStructureCode_C\\dp\\resourced\\data."
      "CSV",
      arr);
  for (int i = 0; i < arr.shape()[0]; i++) {
    for (int j = 0; j < arr.shape()[1]; j++) {
      cout << arr.at(2, i, j) << " ";
    }
    cout << endl;
  }
  NdArray<double> f(0, 2, arr.shape()[0],
                    arr.shape()[1]);  //< 第一行和第一列都没有使用
  NdArray<double> plan(0, 2, arr.shape()[0], arr.shape()[1]);
  double res = max_profile(arr, 6, plan, f);
  cout << "最大利润为：" << res << endl;
  cout << "f：" << endl;
  for (int i = 0; i < f.shape()[0]; i++) {
    for (int j = 0; j < f.shape()[1]; j++) {
      cout << f.at(2, i, j) << " ";
    }
    cout << endl;
  }
  cout << "分配方案为：" << endl;
  for (int i = 0; i < plan.shape()[0]; i++) {
    for (int j = 0; j < plan.shape()[1]; j++) {
      cout << plan.at(2, i, j) << " ";
    }
    cout << endl;
  }
  /**根据f,plan获得最终的策略*/
  int *p = new int[arr.shape()[0]];
  int r = arr.shape()[1] - 1; //< 从plan[n, r]开始
  for (int i = arr.shape()[0] - 1; i >= 1; i--) {
    p[i] = plan.at(2, i, r);
    r -= p[i]; //< 减去分配的资源
  }
  for (int i = 1; i < arr.shape()[0]; i++) {
    cout << "工程" << i << "分配资源" << p[i] << endl;
  }

  return 0;
}
