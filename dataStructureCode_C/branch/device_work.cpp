/**
 * @file device_work.cpp
 * @author your name (you@domain.com)
 * @brief
 * 采用分枝限界法设计工作分配问题：把n项工作分配给n个人，并使分配成本尽可能小。
 * @version 0.1
 * @date 2022-12-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

/**算法思路
 * 1. 用一个二维数组存储每个人干每项工作的成本
 * c[n+1][n+1]，c[i][j]表示第i个人干第j项工作的成本
 * 2.
 * 决策树的每个结点表示一个人干一项工作的情况，每个结点有n个分支，分别表示干第1项、第2项、...、第n项工作的情况
 * 3. 使用代价上界来判断是否剪枝，代价上界的计算方法为：当前最低成本 +
 * 未确定工作的最小成本之和，如果当前代价+未确定工作的最小成本之和大于当前最优解，则剪枝
 */

#include <algorithm>
#include <cstring>
#include <iostream>

using namespace std;

const int MAX_INT = 1e9;  // 最大整数
int output_assignment[5] = {0}; // 用于记录最优的分配结果

/**
 * @brief 限界函数，估计剩余决策的最优解 
 * 
 * @param depth 决策树的深度
 * @param cost 代价
 * @param assignment 用于记录当前的分配结果 
 * @param n 工作数
 * @param c 代价矩阵
 * @return int 
 */
int bound(int depth, int cost, int *assignment, int n, int c[][5]) {
  // 计算剩余的工作数
  int remain = n - depth + 1;
  // 将剩余工作按成本从小到大排序
  sort(c[depth], c[depth] + remain, [](int x, int y) { return x < y; });
  // 将剩余工作分配给成本最小的人
  for (int i = 1; i <= remain; i++) {
    cost += c[depth + i - 1][i - 1];
  }
  return cost;
}

/**
 * @brief 递归函数 
 * 
 * @param depth  决策树的深度
 * @param cost 代价
 * @param upper 代价上界
 * @param n 工作数
 * @param assignment 分配结果
 * @param min_cost 最小成本
 * @param c 代价矩阵
 */
void assign(int depth, int cost, int upper, int n, int *assignment, int &min_cost, int c[][5]) {
  // 如果当前的成本已经大于 min_cost，则直接返回
  if (cost >= min_cost) return;
  // 如果所有工作都已分配，更新最小成本
  if (depth > n) {
    min_cost = min(min_cost, cost);
    return;
  }

  // 计算限界函数的值
  int bound_value = bound(depth, cost, assignment, n, c);
  // 如果限界函数的值已经大于 upper，则剪枝
  if (bound_value >= upper) return;

  // 遍历所有人，尝试分配当前工作
  for (int i = 1; i <= n; i++) {
    // 如果当前人已被分配过工作，则跳过
    if (assignment[i] != 0) continue;

    // 分配工作
    assignment[i] = depth;
    // 递归搜索
    assign(depth + 1, cost + c[i][depth], upper, n, assignment, min_cost, c);
    // 撤销分配回溯
    assignment[i] = 0;
  }
}

int main() {
  int min_cost = MAX_INT;  // 用于记录最小成本
  int c[5][5] = {{0, 0, 0, 0, 0},
                 {0, 9, 2, 7, 8},
                 {0, 6, 4, 3, 7},
                 {0, 5, 8, 1, 8},
                 {0, 7, 6, 9, 4}};  // 用于记录每个人干每项工作的成本
  int n = 4;  // 人数/工作数
  int assignment[n + 1];  // 用于记录每个人分配的工作 assignment[i] = j 表示第i个人干第j项工作
  for (int i = 0; i <= n; i++) assignment[i] = 0; // 初始化
  assign(1, 0, MAX_INT, n,
         assignment, min_cost, c);  // 从第1个工作开始分配，初始成本为0，上界为最大整数
  cout << min_cost << endl;  // 输出最小成本
  return 0;
}
