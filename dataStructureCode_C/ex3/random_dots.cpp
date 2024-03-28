/**
 * @file random_dots.cpp
 * @author your name (you@domain.com)
 * @brief 随机生成100个点
 * @version 0.1
 * @date 2022-10-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <random>
#include <iostream>
#include <time.h>
#include <iostream>
#include "Dot.h"
#include "random_dots.h"

/**
 * @brief 生成随机的点
 * 
 * @param dots 第一维长度为2的二维空数组
 * @param n 需要的点个数
 * @return int 函数执行状态 
 */
int random_dots(Dot dots[], int n) {
    if (dots == NULL || n <= 0) {
        return -1;
    }
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        dots[i] = Dot(rand(), rand());
    }
    return 0;
}
