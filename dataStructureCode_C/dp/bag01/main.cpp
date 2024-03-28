#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>

#include "Graph.h"
#include "NdArray.h"

using namespace std;
using namespace chrono;

/**
 * @brief 单源最短路径，用动态规划法
 *
 * @param g 图
 * @param cost cost[i]
 * 表示从顶点i到终点的最短路径，下标0不是节点编号，节点编号从1开始
 * @param next next[i]
 * 表示从顶点i到终点的最短路径上，i的下一个节点，下标0不是节点编号，节点编号从1开始
 * @param p p[i]
 * 表示最短路径第i次经过的顶点号，下标0不是节点编号，节点编号从1开始
 * @param s 起点
 * @param t 终点
 */
template <typename T>
T shorted_path(Graph<T> &g, T *&cost, int *&next, int *&p, int s, int t) {
  T res;
  int vertexNum = g.get_adj().shape()[0] - 1;
  /**初始化操作，仅在第一次进入该函数的时候执行，后续递归的时候不能执行*/
  if (cost == nullptr) {
    cost = new T[vertexNum + 1];
    cost[t] = 0;
    /**从1开始赋值*/
    for (int i = 1; i <= vertexNum; i++) {
      cost[i] = g.get_adj().at(2, i, t);
    }
  } else {
    cost[t] = 0;
    /**从1开始赋值*/
    for (int i = 1; i <= vertexNum; i++) {
      cost[i] = g.get_adj().at(2, i, t);
    }
  }
  if (next == nullptr) {
    next = new int[vertexNum + 1];
    for (int i = 1; i <= vertexNum; i++) {
      /**0表示未知下一个点的编号*/
      next[i] = 0;
    }
  } else {
    for (int i = 1; i <= vertexNum; i++) {
      /**0表示未知下一个点的编号*/
      next[i] = 0;
    }
  }
  if (p == nullptr) {
    p = new int[vertexNum + 1];
    for (int i = 1; i <= vertexNum; i++) {
      /**0表示未知下一个点的编号*/
      p[i] = 0;
    }
  } else {
    for (int i = 1; i <= vertexNum; i++) {
      /**0表示未知下一个点的编号*/
      p[i] = 0;
    }
  }

  /**划分最优化重叠子问题，从s走到终点t的最短路径问题的最优子问题可以分解
   * cost[i] = min{cost[j_1] + adj[i][j_1], cost[j_2] + adj[i][j_2],
   * ...}，其中j的集合是i的后继节点集合
   * 遍历顺序：按照以终点t为起点的广度优先搜索算法
   */
  int *pre = g.get_pre(t);
  /** 此处判断是否可达，如果s不在t为起点的广度优先搜索中*/
  int reachable = 0;
  for (int i = 1; i <= vertexNum; i++) {
    if (pre[i] == s) {
      /**如果s可达t*/
      reachable = 1;
      break;
    }
  }
  if (reachable == 0) {
    /**如果s不可达t*/
    cout << "s不可达t" << endl;
    return numeric_limits<T>::max();
  }
  int min = numeric_limits<T>::max();
  for (int i = 1; i <= pre[0]; i++) {
    for (int m = 1; m <= vertexNum; m++) {
      /**找到点pre[i]的后继节点集*/
      /**判断cost[m]是为了防止cost[m] + g.get_adj().at(2 ...)溢出*/
      if (g.get_adj().at(2, pre[i], m) != numeric_limits<T>::max() &&
          cost[m] != numeric_limits<T>::max()) {
        /**如果cost[m] + adj[pre[i]][m] <
         * cost[pre[i]]，则更新cost[pre[i]]，不加等于会导致漏掉到12的同时也会让长度相等的路径覆盖*/
        if (cost[m] + g.get_adj().at(2, pre[i], m) <= cost[pre[i]]) {
          cost[pre[i]] = cost[m] + g.get_adj().at(2, pre[i], m);
          next[pre[i]] = m;
        }
      }
    }
  }
  /**从next[i]中获取p，起点是s*/
  int j = s;
  int k = 1;
  /** 当走到终点时停止*/
  while (next[j] != t) {
    p[k++] = j;
    j = next[j];
  }
  p[k++] = j;
  j = next[j];
  p[k] = t;
  p[0] = k;
  return cost[s];
}

// int main() {
//   // test-NdArray------------
//   /**构造二维3x4的int类型数据对象，初始化为0*/
//   // NdArray<int> arr(0, 2, 3, 4);
//   // cout << arr.to_string() << endl;
//   // arr.assign(1, 0, 0);
//   // cout << arr.to_string() << endl;
//   // int temp = arr.at(2, 0, 0);
//   // cout << temp << endl;
//   // arr.assign_all(2);
//   // cout << arr.to_string() << endl;
//   // NdArray<int> arr2(arr);
//   // cout << arr2.to_string() << endl;

//   // arr.~NdArray();
//   // arr2.~NdArray();
//   // test-Graph------------
//   // NdArray<int> c(0, 2, 13, 13);
//   // Graph<int> g(c);
//   // cout << g.get_adj().to_string() << endl;
//   // c.assign(1, 0, 1);
//   // cout << c.to_string() << endl;
//   // g.set_adj(c);
//   // cout << g.get_adj().to_string() << endl;
//   // Graph<int>
//   // g2("E:\\pros\\dataStructureCode_C\\dataStructureCode_C\\dp\\bag01\\graph.csv",
//   // c); cout << g2.get_adj().to_string() << endl;
//   // int * pre = g2.get_pre(12);
//   // for (int i = 1; i <= pre[0]; i++) {
//   //   cout << pre[i] << " ";
//   // }
//   // ------------

//   int *cost = nullptr;
//   int *next = nullptr;
//   int *p = nullptr;
//   /**代价矩阵(二维矩阵结构)*/
//   NdArray<int> c(0, 2, 13, 13);
//   Graph<int> g(
//       "E:\\pros\\dataStructureCode_C\\dataStructureCode_C\\dp\\bag01\\graph."
//       "csv",
//       c);

//   system_clock::time_point start = system_clock::now();
//   // shorted_path(g, cost, next, p, 1, 12);
//   // int len = shorted_path(g, cost, next, p, 1, 3);
//   int len = shorted_path(g, cost, next, p, 4, 10);
//   system_clock::time_point end = system_clock::now();
//   cout << "time: " << duration_cast<microseconds>(end - start).count() / 1e6
//        << "s" << endl;
//   cout << "len: " << len << endl;

//   cout << "cost:" << cost[0] << "| ";
//   for (int i = 1; i <= 12; i++) {
//     cout << cost[i] << " ";
//   }
//   cout << endl << "next:" << next[0] << "| ";
//   for (int i = 1; i <= 12; i++) {
//     cout << next[i] << " ";
//   }
//   cout << endl << "p:" << p[0] << "| ";
//   for (int i = 1; i <= 12; i++) {
//     cout << p[i] << " ";
//   }
//   /**随机种子*/
//   srand((unsigned)time(NULL));
//   /**随机选择起点和终点*/
//   for (int i = 0; i < 10; i++) {
//     int s = rand() % 12 + 1;
//     int t = rand() % 12 + 1;
//     cout << endl << "s:" << s << " t:" << t << endl;
//     system_clock::time_point start2 = system_clock::now();
//     int len = shorted_path(g, cost, next, p, s, t);
//     system_clock::time_point end2 = system_clock::now();
//     cout << "time: " << duration_cast<microseconds>(end2 - start2).count() / 1e6
//          << "s" << endl;
//     cout << "len: " << len << endl;

//     cout << "cost:" << cost[0] << "| ";
//     for (int i = 1; i <= 12; i++) {
//       cout << cost[i] << " ";
//     }
//     cout << endl << "next:" << next[0] << "| ";
//     for (int i = 1; i <= 12; i++) {
//       cout << next[i] << " ";
//     }
//     cout << endl << "p:" << p[0] << "| ";
//     for (int i = 1; i <= 12; i++) {
//       cout << p[i] << " ";
//     }
//     cout << endl << "---------------------------------" << endl;
//   }
  
//   return 0;
// }
