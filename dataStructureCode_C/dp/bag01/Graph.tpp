/**
 * @file Graph.tpp
 * @author your name (you@domain.com)
 * @brief Graph类的实现 
 * @version 0.1
 * @date 2022-12-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <fstream>
#include <queue>
#include <sstream>

#include "Graph.h"

template <typename T>
void Graph<T>::read_csv(std::string filename, NdArray<T> &arr) {
  ifstream file(filename);
  string line;
  int i = 0;
  while (getline(file, line)) {
    stringstream ss(line);
    string cell;
    int j = 0;
    while (getline(ss, cell, ',')) {
      /**如果cell等于-1则*/
      if (cell == "-1") {
        //< 取最大值(可认为是无穷大)
        arr.assign(numeric_limits<T>::max(), i, j);
      } else {
        arr.assign(stod(cell), i, j);
      }
      j++;
    }
    i++;
  }
}

template <typename T>
inline Graph<T>::~Graph() {
  adj.~NdArray();
  delete[] pre;
}

template <typename T>
inline Graph<T>::Graph(NdArray<T> &arr) : adj(arr) {
  vertex_num = arr.shape()[0] - 1;
  edge_num = 0;
  for (int i = 0; i < vertex_num; i++) {
    for (int j = 0; j < vertex_num; j++) {
      if (this->adj.at(2, i, j) != numeric_limits<T>::max()) {
        edge_num++;
      }
    }
  }
  pre = new int[vertex_num + 1];
}

template <typename T>
inline Graph<T>::Graph(std::string path, NdArray<T> &arr) {
  this->set_adj(arr);
  vertex_num = arr.shape()[0] - 1;
  edge_num = 0;
  for (int i = 0; i < vertex_num; i++) {
    for (int j = 0; j < vertex_num; j++) {
      if (this->adj.at(2, i, j) != numeric_limits<T>::max()) {
        edge_num++;
      }
    }
  }
  pre = new int[vertex_num + 1];
  read_csv(path, this->adj);
}

/**
 * @brief 广度优先搜索
 *
 * @tparam T
 * @param v 输入节点编号
 * @return T* 返回广度优先搜索的结果，T[1]存储数量
 */
template <typename T>
inline T *Graph<T>::get_pre(int v) {
  /**最多有vertex_num个前驱*/
  /**这里每次执行都会新建空间比较影响性能，因此将pre提升到类成员能提高效率,pre在这里仅仅作为一个空间，因此不提供getset方法*/
  for (int i = 0; i <= vertex_num; i++) {
    pre[i] = -1;
  }
  /**广度优先遍历*/
  int *visited = new int[vertex_num + 1];
  for (int i = 1; i <= vertex_num; i++) {
    visited[i] = 0;
  }
  int k = 1;
  queue<int> q;
  q.push(v);
  while (q.size() != 0) {
    int cur = q.front();
    q.pop();
    /** 若已经访问过*/
    if (visited[cur] == 1) {
      continue;
    }
    /**未访问过才执行*/
    visited[cur] = 1;  //< 表示已经访问过了
    pre[k++] = cur;
    for (int i = 1; i <= vertex_num; i++) {
      /**从邻接矩阵中找前趋*/
      if (adj.at(2, i, cur) != numeric_limits<T>::max() && visited[i] == 0 &&
          adj.at(2, i, cur) != 0) {
        q.push(i);
      }
    }
  }
  /**pre[0]记录遍历到的点的树木*/
  pre[0] = k - 1;

  return pre;
}
