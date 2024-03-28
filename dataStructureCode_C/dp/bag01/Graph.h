/**
 * @file Graph.h
 * @author your name (you@domain.com)
 * @brief 有向图的邻接表表示
 * @version 0.1
 * @date 2022-11-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include "NdArray.h"
#include <limits>

template<typename T>
class Graph {
 private:
  int vertex_num;  //< 顶点数
  int edge_num;    //< 边数
  NdArray<T> adj; //< 邻接矩阵
  T* pre;         //< 前趋顶点数组，为函数get_pre提供临时空间
 public:
  ~Graph();
  Graph(NdArray<T> &arr);
  Graph(int vertex_num);
  Graph(std::string path, NdArray<T> &arr);
  void add_edge(int v, int w);
  std::string to_string();
  void read_csv(std::string path, NdArray<T> &arr);
  T *get_pre(int v);

  int get_vertex_num() const { return vertex_num; }
  void set_vertex_num(int vertex_num_) { vertex_num = vertex_num_; }

  int get_edge_num() const { return edge_num; }
  void set_edge_num(int edge_num_) { edge_num = edge_num_; }

  NdArray<T> get_adj() const { return adj; }
  void set_adj(const NdArray<T> &adj_) { adj = adj_; }

};
#include "Graph.tpp"
#endif  // GRAPH_H