/**
 * @file shortest_dij.cpp
 * @author your name (you@domain.com)
 * @brief
 * 用Dijkstra算法解决单源最短路径问题，并给出算法的时间复杂性分析。用你写的程序求出下图节点1到任何一个节点的最短路径，包括最短路径值及其最短路径
 * @version 0.1
 * @date 2022-12-25
 *
 * @copyright Copyright (c) 2022
 *
 */

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

// 算法思路：
/**
 * 算法思路:
算法基于贪心策略，每次选择离源点最近的未被访问的节点，并更新所有与该节点相连的边的最短路径。
 * 1. 初始化：将所有节点的最短路径设为无穷大，将源点的最短路径设为0。
 * 2. 找到未被访问的节点中离源点最近的节点u。
 * 3.
对于所有与u相连的边(u,v)，如果通过边(u,v)的路径比v的当前最短路径短，则更新v的最短路径为通过边(u,v)的路径。
 * 4. 将节点u标记为已访问。
 * 重复2-4，直到所有节点都被访问。
 *
 * /

/**
 * @brief 用Dijkstra算法求解单源最短路径问题
 *
 * @param g 输入的图
 * @param cost 存储源点到各个节点的最短路径
 * @param pre pre[i]表示顶点i的前驱顶点
 * @param p p[i]最短路径第i次经过的顶点
 * @param s 起点
 * @param t 终点
 * @return int
 */
int shortest_path_dijkstra(Graph<int> &g, int *cost, int *pre, int *p, int s,
                           int t) {
  int n = g.get_vertex_num();
  int *visited = new int[n + 1];

  for (int i = 0; i <= n; i++) {
    visited[i] = 0;
    cost[i] = numeric_limits<int>::max();
    pre[i] = 0;
    p[i] = 0;
  }
  cost[s] = 0;
  pre[s] = s;
  int visited_num = 0;
  p[visited_num++] = s;
  int u = s;
  while (u != 0) {
    visited[u] = 1;
    for (int v = 1; v <= n; v++) {
      if (g.get_adj().at(2, u, v) != 0 && visited[v] == 0 &&
          g.get_adj().at(2, u, v) != numeric_limits<int>::max()) {
        if (cost[u] + g.get_adj().at(2, u, v) < cost[v]) {
          cost[v] = cost[u] + g.get_adj().at(2, u, v);
          pre[v] = u;
        }
      }
    }
    u = 0;
    int min = numeric_limits<int>::max();
    // 选择一个未访问的顶点，且到源点的距离最短，作为下一个访问的顶点
    for (int i = 1; i <= n; i++) {
      if (visited[i] == 0 && cost[i] < min) {
        min = cost[i];
        u = i;
      }
    }
  }
  /** 从pre[i]中获取p，起点是s*/
  int j = t;
  int k = 12;
  p[k--] = t;
  while (pre[j] != s) {
    p[k--] = pre[j];
    j = pre[j];
  }
  p[k--] = s;
  j = pre[j];
  p[0] = 12 - k;
    
  delete[] visited;
  return cost[t];
}

int main() {
  int *cost = nullptr;
  int *pre = nullptr;
  int *p = nullptr;
  int n = 12;
  if (cost == nullptr) {
    cost = new int[n + 1];
    for (int i = 0; i <= n; i++) {
      cost[i] = numeric_limits<int>::max();
    }
  }
  if (pre == nullptr) {
    pre = new int[n + 1];
    for (int i = 0; i <= n; i++) {
      pre[i] = 0;
    }
  }
  if (p == nullptr) {
    p = new int[n + 1];
    for (int i = 0; i <= n; i++) {
      p[i] = 0;
    }
  }
  /**代价矩阵(二维矩阵结构)*/
  NdArray<int> c(0, 2, 13, 13);
  Graph<int> g(
      "E:\\pros\\dataStructureCode_C\\dataStructureCode_C\\dp\\bag01\\graph."
      "csv",
      c);

  system_clock::time_point start = system_clock::now();
  int len = shortest_path_dijkstra(g, cost, pre, p, 1, 12);
  system_clock::time_point end = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end - start).count() / 1e6
       << "s" << endl;
  cout << "len: " << len << endl;

  cout << "cost:";
  for (int i = 1; i <= 12; i++) {
    cout << cost[i] << " ";
  }
  cout << endl << "pre:";
  for (int i = 1; i <= 12; i++) {
    cout << pre[i] << " ";
  }
  cout << endl << "p:";
  for (int i = 1; i <= 12; i++) {
    cout << p[i] << " ";
  }
  /**随机种子*/
  srand((unsigned)time(NULL));
  /**随机选择终点*/
  for (int i = 0; i < 10; i++) {
    int s = 1;
    int t = rand() % 12 + 1;
    cout << endl << "s:" << s << " t:" << t << endl;
    system_clock::time_point start2 = system_clock::now();
    int len = shortest_path_dijkstra(g, cost, pre, p, s, t);
    system_clock::time_point end2 = system_clock::now();
    cout << "time: " << duration_cast<microseconds>(end2 - start2).count() / 1e6
         << "s" << endl;
    cout << "len: " << len << endl;

    cout << "cost:" << cost[0] << "| ";
    for (int i = 1; i <= 12; i++) {
      cout << cost[i] << " ";
    }
    cout << endl << "pre:" << pre[0] << "| ";
    for (int i = 1; i <= 12; i++) {
      cout << pre[i] << " ";
    }
    cout << endl << "p:" << p[0] << "| ";
    for (int i = 1; i <= 12; i++) {
      cout << p[i] << " ";
    }
    cout << endl << "---------------------------------" << endl;
  }

  return 0;
}
