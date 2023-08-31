// /**
//  * @file min_distance_brute_force.cpp
//  * @author your name (you@domain.com)
//  * @brief 分别用蛮力法和分治法求解点集中的最近点对
//  * @version 0.1
//  * @date 2022-10-21
//  *
//  * @copyright Copyright (c) 2022
//  *
//  */

// #include <algorithm>
// #include <chrono>
// #include <iostream>
// #include <limits>

// #include "Dot.h"
// #include "random_dots.h"

// using namespace std;
// using namespace chrono;

// /**
//  * @brief 蛮力法求解点集中的最近点对
//  *
//  * @param dots 输入点集
//  * @param n 点集点数量
//  * @param dot_pair
//  * 接收最近点对的数组，长度为2，由于C++局部数组无法返回，只要出其范围就会被销毁，所以这里用外部输入的指针传递
//  * @return int
//  */
// int min_distance_brute_force(Dot dots[], int n, Dot dot_pair[]) {
//   if (dots == NULL || n <= 0 || dot_pair == NULL) {
//     cout << "input error" << endl;
//     return -1;
//   }
//   /**初始化dot_pair*/
//   dot_pair[0] = dots[0];
//   dot_pair[1] = dots[1];
//   /**临时存储点对*/
//   for (int i = 0; i < n; i++) {
//     for (int j = i + 1; j < n; j++) {
//       /**如果距离更近就更新点对dot_pair*/
//       if (Dot::distance(dots[i], dots[j]) <
//           Dot::distance(dot_pair[0], dot_pair[1])) {
//         dot_pair[0] = dots[i];
//         dot_pair[1] = dots[j];
//       }
//     }
//   }
//   return 0;
// }

// double min_distance_deviced(Dot dots[], int head, int tail, Dot dot_pair[]) {
//   int len = tail - head + 1;
//   if (dots == NULL || len <= 0 || dot_pair == NULL) {
//     cout << "input error" << endl;
//     return -1;
//   }
//   double res = numeric_limits<double>::max();  //< double的最大值
//   /**边界条件*/
//   /**如果点集不够成对*/
//   if (len < 2) {
//     return res;
//   } else if (len == 2) {
//     dot_pair[0] = dots[head];
//     dot_pair[1] = dots[tail];
//     return Dot::distance(dots[head], dots[tail]);
//   }

//   /**划分治理*/
//   Dot dot_pair_l[2];
//   Dot dot_pair_r[2];
//   /**始终使用同一个dots作为输入，极大减少重新分配空间的开销*/
//   double min_l = min_distance_deviced(dots, head, head + len / 2, dot_pair_l);
//   double min_r =
//       min_distance_deviced(dots, head + len / 2 + 1, tail, dot_pair_r);

//   /**合并*/
//   if (min_l < min_r) {
//     dot_pair[0] = dot_pair_l[0];
//     dot_pair[1] = dot_pair_l[1];
//     res = min_l;
//   } else {
//     dot_pair[0] = dot_pair_r[0];
//     dot_pair[1] = dot_pair_r[1];
//     res = min_r;
//   }

//   /**计算中间带的最近点对，按照dots[head + len/2].get_x()来区分*/
//   Dot dots_m[len];
//   double mid_x = dots[head + len / 2].get_x();
//   int j = 0;  //< 记录dots_m的下标
//   for (int i = head; i <= tail; i++) {
//     if (abs(dots[i].get_x() - mid_x) < res) {
//       dots_m[j++] = dots[i];
//       // try {
//       //     cout << "dots_m[" << j - 1 << "] = " << dots_m[j - 1].to_string()
//       //     << endl;
//       // } catch (const std::exception& e) {
//       //     std::cerr << e.what() << '\n';
//       // }
//     }
//   }
//   /**dots_m中间带的点按照y值排序*/
//   sort(dots_m, dots_m + j, [](Dot a, Dot b) { return a.get_y() < b.get_y(); });
//   double temp_dis = 0;
//   Dot dots_6[7];  //< 取六个点
//   Dot temp_dot;   //< 临时点
//   /**遍历dots_m选择左边的点集做遍历，每次与对边y方向相邻的6个点求距离*/
//   int z = 0;
//   for (int i = 0; i < j; i++) {
//     if (dots_m[j].get_x() - mid_x >= 0) {
//         continue;
//     }
//     int x = 0;
//     for (z = i + 1; z <= i + 6 + x && z < j; z++) {
//         if (dots_m[z].get_x() - mid_x < 0) {
//             x++;
//             continue;
//         }
//         if (Dot::distance(dots_m[i], dots_m[z]) < res) {
//             dot_pair[0] = dots_m[i];
//             dot_pair[1] = dots_m[z];
//             res = Dot::distance(dots_m[i], dots_m[z]);
//         }
//     }
//   }

//   return res;
// }

// int main() {
//   /**声明一个二维且第一维度长为2的点集*/
//   Dot dots[100];
//   Dot dot_pair_1[2];
//   Dot dot_pair_2[2];

//   srand((unsigned)time(NULL));
//   /**记录结果相同的次数*/
//   int equ_times = 0;
//   for (int i = 0; i < 100; i++) {
//     /**随机种子在循环内失效*/
//     for (int i = 0; i < 100; i++) {
//       dots[i] = Dot(rand(), rand());
//     }
//     /**使用匿名函数和sort快排，按照x对点排序*/
//     sort(dots, dots + 100, [](Dot a, Dot b) { return a.get_x() < b.get_x(); });

//     system_clock::time_point start = system_clock::now();
//     min_distance_brute_force(dots, 100, dot_pair_1);
//     system_clock::time_point end = system_clock::now();
//     cout << "time: " << duration_cast<microseconds>(end - start).count() / 1e6
//          << "s" << endl;
//     cout << "min distance: " << Dot::distance(dot_pair_1[0], dot_pair_1[1])
//          << endl;
//     cout << "dot1: " << dot_pair_1[0].to_string() << endl;
//     cout << "dot2: " << dot_pair_1[1].to_string() << endl;

//     system_clock::time_point start_2 = system_clock::now();
//     // min_distance_brute_force(dots, 100, dot_pair);
//     min_distance_deviced(dots, 0, 99, dot_pair_2);
//     system_clock::time_point end_2 = system_clock::now();
//     cout << "time: "
//          << duration_cast<microseconds>(end_2 - start_2).count() / 1e6 << "s"
//          << endl;
//     cout << "min distance: " << Dot::distance(dot_pair_2[0], dot_pair_2[1])
//          << endl;
//     cout << "dot1: " << dot_pair_2[0].to_string() << endl;
//     cout << "dot2: " << dot_pair_2[1].to_string() << endl;

//     if (Dot::distance(dot_pair_1[0], dot_pair_1[1]) ==
//             Dot::distance(dot_pair_2[0], dot_pair_2[1]) &&
//         dot_pair_1[0] == dot_pair_2[0] && dot_pair_1[1] == dot_pair_2[1]) {
//       equ_times++;
//     }
//   }
//   cout << "equ_times: " << equ_times << endl;
//   return 0;
// }
