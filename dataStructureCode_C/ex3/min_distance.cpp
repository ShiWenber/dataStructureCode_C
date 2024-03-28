/**
 * @file min_distance_brute_force.cpp
 * @author your name (you@domain.com)
 * @brief 分别用蛮力法和分治法求解点集中的最近点对
 * @version 0.1
 * @date 2022-10-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>

#include "Dot.h"
#include "random_dots.h"

using namespace std;
using namespace chrono;

/**
 * @brief 蛮力法求解点集中的最近点对
 *
 * @param dots 输入点集
 * @param n 点集点数量
 * @param dot_pair
 * 接收最近点对的数组，长度为2，由于C++局部数组无法返回，只要出其范围就会被销毁，所以这里用外部输入的指针传递
 * @return double
 */
double min_distance_brute_force(Dot dots[], int n, Dot dot_pair[]) {
  if (dots == NULL || n <= 0 || dot_pair == NULL) {
    cout << "input error" << endl;
    return -1;
  }
  /**初始化dot_pair*/
  double min_dis = numeric_limits<double>::max();
  double dis = 0;
  /**临时存储点对*/
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      /**如果距离更近就更新点对dot_pair*/
      dis = Dot::distance(dots[i], dots[j]);
      if (dis < min_dis) {
        dot_pair[0] = dots[i];
        dot_pair[1] = dots[j];
        min_dis = dis;
      }
    }
  }
  return min_dis;
}

/**
 * @brief 分治法求解点集中的最近点对
 *
 * @param dots 输入点集
 * @param head 输入点集开始下标
 * @param tail 输入点集结束下标
 * @param dot_pair 最近点对
 * @return double 最近距离
 */
double min_distance_deviced(Dot dots[], int head, int tail, Dot dot_pair[]) {
  int len = tail - head + 1;
  if (dots == NULL || len <= 0 || dot_pair == NULL) {
    cout << "input error" << endl;
    return -1;
  }
  double res = numeric_limits<double>::max();  //< double的最大值
  /**边界条件*/
  /**如果点集不够成对*/
  if (len < 2) {
    return res;
  } else if (len == 2) {
    dot_pair[0] = dots[head];
    dot_pair[1] = dots[tail];
    return Dot::distance(dots[head], dots[tail]);
  }

  /**划分治理*/
  Dot dot_pair_l[2];
  Dot dot_pair_r[2];
  /**始终使用同一个dots作为输入，极大减少重新分配空间的开销*/
  double min_l = min_distance_deviced(dots, head, head + len / 2, dot_pair_l);
  double min_r =
      min_distance_deviced(dots, head + len / 2 + 1, tail, dot_pair_r);

  /**合并*/
  if (min_l < min_r) {
    dot_pair[0] = dot_pair_l[0];
    dot_pair[1] = dot_pair_l[1];
    res = min_l;
  } else {
    dot_pair[0] = dot_pair_r[0];
    dot_pair[1] = dot_pair_r[1];
    res = min_r;
  }

  /**计算中间带的最近点对，按照dots[head + len/2].get_x()来区分*/
  Dot dots_m[len];
  double mid_x = dots[head + len / 2].get_x();
  int j = 0;  //< 记录dots_m的下标
  for (int i = head; i <= tail; i++) {
    if (abs(dots[i].get_x() - mid_x) < res) {
      dots_m[j++] = dots[i];
      // try {
      //     cout << "dots_m[" << j - 1 << "] = " << dots_m[j - 1].to_string()
      //     << endl;
      // } catch (const std::exception& e) {
      //     std::cerr << e.what() << '\n';
      // }
    }
  }
  /**dots_m中间带的点按照y值排序*/
  sort(dots_m, dots_m + j, [](Dot a, Dot b) { return a.get_y() < b.get_y(); });
  double temp_dis = 0;
  double min_dis = res;
  Dot temp_dot;   //< 临时点
  /**遍历dots_m选择左边的点集做遍历，每次与对边y方向相邻的6个点求距离*/
  for (int i = 0; i < j; i++) {
    /**每个点都往下取6个点，因为遍历所有点所以不必考虑上下取点*/
    for (int k = i + 1; k < j && k <= i + 5; k++) {
      temp_dis = Dot::distance(dots_m[i], dots_m[k]);
      if (temp_dis < min_dis) {
        min_dis = temp_dis;
        dot_pair[0] = dots_m[i];
        dot_pair[1] = dots_m[k];
      }
    }
  }

  return std::min<double>(res, min_dis);
}

int main() {
  int len = 1000;
  /**声明一个二维且第一维度长为2的点集*/
  Dot dots[len];
  Dot dot_pair_1[2];
  Dot dot_pair_2[2];
  ofstream fout("data.txt");

  srand((unsigned)time(NULL));
  /**记录结果相同的次数*/
  int equ_times = 0;

  for (int i = 0; i < 1000; i++) {
    /**随机种子在循环内失效*/
    for (int i = 0; i < len; i++) {
      dots[i] = Dot(rand(), rand());
    }
    /**使用匿名函数和sort快排，按照x对点排序*/
    sort(dots, dots + len, [](Dot a, Dot b) { return a.get_x() < b.get_x(); });

    system_clock::time_point start = system_clock::now();
    double min_dis1 = min_distance_brute_force(dots, len, dot_pair_1);
    system_clock::time_point end = system_clock::now();
    cout << "-time: " << duration_cast<microseconds>(end - start).count() / 1e6
         << "s" << endl;
    cout << "min distance: " << Dot::distance(dot_pair_1[0], dot_pair_1[1])
         << endl;
    cout << "dot1: " << dot_pair_1[0].to_string() << endl;
    cout << "dot2: " << dot_pair_1[1].to_string() << endl;

    system_clock::time_point start_2 = system_clock::now();
    // min_distance_brute_force(dots, len, dot_pair);
    double min_dis2 = min_distance_deviced(dots, 0, len - 1, dot_pair_2);
    system_clock::time_point end_2 = system_clock::now();
    cout << "time: "
         << duration_cast<microseconds>(end_2 - start_2).count() / 1e6 << "s"
         << endl;
    cout << "min distance: " << Dot::distance(dot_pair_2[0], dot_pair_2[1])
         << endl;
    cout << "dot1: " << dot_pair_2[0].to_string() << endl;
    cout << "dot2: " << dot_pair_2[1].to_string() << endl;

    if (Dot::distance(dot_pair_1[0], dot_pair_1[1]) ==
                Dot::distance(dot_pair_2[0], dot_pair_2[1]) &&
            (dot_pair_1[0] == dot_pair_2[0] &&
             dot_pair_1[1] == dot_pair_2[1]) ||
        (dot_pair_1[0] == dot_pair_2[1] && dot_pair_1[1] == dot_pair_2[0])) {
      equ_times++;
    } else {
      for (int i = 0; i < len; i++) {
        fout << dots[i].to_string();
      }
      fout << endl
           << "min distance: " << Dot::distance(dot_pair_1[0], dot_pair_1[1])
           << endl;
    }
  }
  fout.close();
  cout << "equ_times: " << equ_times << endl;
  return 0;
}