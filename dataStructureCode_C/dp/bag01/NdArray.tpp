/**
 * @file NdArray.tpp
 * @author your name (you@domain.com)
 * @brief NdArray类的实现 
 * @version 0.1
 * @date 2022-12-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdarg.h>
#include <iostream>
#include <string>

#include "NdArray.h"

using namespace std;

template <typename T>
inline NdArray<T>::NdArray() {
  this->dim = 0;
  this->bounds = NULL;
  this->constants = NULL;
  this->base = NULL;
  this->total = 0;
}

/**
 * @brief Construct a new Nd Array:: Nd Array object,
 * 例如对于3x3的int数组，NdArray<int>(0, 2, 3, 3)。
 *
 * @tparam T 元素类型
 * @param elm 初始元素
 * @param dim 维度
 * @param ... 每个维度的长度
 *
 *
 */
template <typename T>
inline NdArray<T>::NdArray(const T& elm, int dim, ...) {
  if (dim < 1) {
    return;
  }
  this->dim = dim;
  this->bounds = new int[dim];
  this->constants = new int[dim];
  va_list ap;
  va_start(ap, dim);
  this->total = 1;
  for (int i = 0; i < dim; i++) {
    this->bounds[i] = va_arg(ap, int);
    /**验证this->bounds[i]是否小于0，如果小于0抛出一个异常*/
    if (this->bounds[i] < 0) {
      std::string exp_str =
          "The " + std::to_string(i + 1) + "th dimension is less than 0";
      throw exp_str;
    }
    this->total *= this->bounds[i];
  }
  va_end(ap);
  this->base = new T[this->total];
  for (int i = 0; i < this->total; i++) {
    this->base[i] = elm;
  }
  this->constants[dim - 1] = 1;  //< 最后一维的一个单元只能防置一个元素
  for (int i = dim - 2; i >= 0; i--) {
    this->constants[i] = this->bounds[i + 1] * this->constants[i + 1];
  }
}

/**
 * @brief Construct a new Nd Array< T>:: Nd Array object，拷贝构造函数
 *
 * @tparam T
 * @param arr
 */
template <typename T>
inline NdArray<T>::NdArray(const NdArray<T>& arr) {
  this->dim = arr.dim;
  this->bounds = new int[this->dim];
  this->constants = new int[this->dim];
  this->total = arr.total;
  for (int i = 0; i < this->dim; i++) {
    this->bounds[i] = arr.bounds[i];
    this->constants[i] = arr.constants[i];
  }
  this->base = new T[this->total];
  for (int i = 0; i < this->total; i++) {
    this->base[i] = arr.base[i];
  }
}

template <typename T>
inline NdArray<T>::~NdArray() {}

/**
 * @brief 获取连续空间中相对位置的辅助函数，不提供给外部使用，可以设置为private
 * 
 * @tparam T 
 * @param ap 
 * @return int 
 */
template <typename T>
inline int NdArray<T>::locate(va_list ap) {
  int reAddress = 0;
  for (int i = 0; i < this->dim; i++) {
    int index = va_arg(ap, int);
    if (index < 0 || index >= this->bounds[i]) {
      cerr << "The " << i + 1 << "th index is out of bounds" << endl;
      throw "index out of range";
      return -1;
    }
    reAddress += index * this->constants[i];
  }
  return reAddress;
}

/**
 * @brief 用法：.at(2, 1, 2)，取维度为2的数组中的第2行第3列的元素
 * 
 * @tparam T 
 * @param dim 维度
 * @param ... 定位下标，参数数量和维度相同
 * @return T 
 */
template <typename T>
inline T NdArray<T>::at(int dim, ...) {
  va_list ap;
  va_start(ap, dim);
  try {
    int reAddress = NdArray::locate(ap);
    T elm = this->base[reAddress];
    va_end(ap);
    return elm;
  } catch (const std::exception& e) {
    std::cerr << e.what() << endl;
    return T();
  }
}

/**
 * @brief 赋值函数
 *
 * @tparam T
 * @param e 赋值的值
 * @param ... 下标序列
 * @return int
 */
template <typename T>
inline int NdArray<T>::assign(const T& e, ...) {
  va_list ap;
  va_start(ap, e);
  try {
    int reAddress = NdArray::locate(ap);
    this->base[reAddress] = e;
    va_end(ap);
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << endl;
    return -1;
  }
}
/**
 * @brief 对数组做全局赋值
 * 
 * @tparam T 
 * @param e 
 * @return int 
 */
template <typename T>
inline int NdArray<T>::assign_all(const T& e) {
  if (this->dim == 0) {
    cout << "null" << endl;
    return -1;
  }
  for (int i = 0; i < this->total; i++) {
    this->base[i] = e;
  }
  return 0;
}

template <typename T>
inline string NdArray<T>::to_string() {
  std::string res;
  if (this->dim > 2) {
    // return  "数组维度超过二维，无法展示";
  } else if (this->dim == 0) {
    return "数组为空";
  }
  for (int i = 0; i < this->total; i++) {
    res += std::to_string(this->base[i]) + "\t";
    if (i % this->bounds[this->dim - 1] == this->bounds[this->dim - 1] - 1) {
      res += "\n";
    }
  }
  return res;
}

/**
 * @brief 返回数组的维度
 *
 * @tparam T
 * @return int* 返回数组维度, 从0开始
 */
template <typename T>
inline int* NdArray<T>::shape() {
  return this->bounds;
}