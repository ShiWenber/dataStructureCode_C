/**
 * @file NdArray.h
 * @author your name (you@domain.com)
 * @brief 具有连续空间的任意维度数组类型，参考了 numpy 的 ndarray
 * 以及数据结构上对多维数组的描述
 * @version 0.1
 * @date 2022-11-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef NDARRAY_H
#define NDARRAY_H
#include <stdarg.h>

#include <iostream>
#include <string>
// 模板函数也可内联
template <class T>
class NdArray {
 private:
  T *base;  //< 空间基址
  int dim;  //< 维数
  int *
      bounds;  //<数组维界，存储每个维度的最大元素个数，bounds[i]表示第i+1维的最大元素个数
  int *
      constants;  //<数组映像函数常量，constants[i]表示第i+1维的第一个元素在总空间中的偏移量，根据映像函数的公式，constants[i]= bounds[i+1] * constants[i+1], constants[dim-1] =1(表示一个T)bounds和constants[dim-1]是容易知道的，因此求constants[i]时，从dim-1开始，依次求constants[i]，直到constants[0]为止
  int total;      //< 总空间T元素总数
 public:
  NdArray();                            //< 默认构造函数
  NdArray(const T &elm, int dim, ...);  //< 构造函数
  NdArray(const NdArray<T> &ndarray);   //< 拷贝构造函数
  ~NdArray();                           //< 析构函数
  int locate(va_list ap);               //< 求相对地址的辅助函数
  T at(int i,
       ...);  //< 定位取出某位置的元素 TODO: 重载[]运算符？？也可能无法实现？
  int assign(const T &elm, ...);  //< 给某位置的元素赋值
  std::string to_string();               //< 循环输出
  int assign_all(const T &elm);          //< 批量赋值
  int* shape();                          //< 返回维界

  T *get_base() const { return base; }
  void set_base(T *base_) { base = base_; }

  int get_dim() const { return dim; }
  void set_dim(int dim_) { dim = dim_; }

  int * get_bounds() const { return bounds; }
  void set_bounds(int * bounds_) { bounds = bounds_; }

  int * get_constants() const { return constants; }
  void set_constants(int * constants_) { constants = constants_; }



};

#include "NdArray.tpp" //< 模板类的实现文件，模板类无法头文件和实现分离编译
#endif  // !NDARRAY_H