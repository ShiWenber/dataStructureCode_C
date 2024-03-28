/**
 * @file Dot.cpp
 * @author your name (you@domain.com)
 * @brief 坐标点数据类型
 * @version 0.1
 * @date 2022-10-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <math.h>
#include "Dot.h"

/**
 * @brief 点对象的实现
 *
 */
Dot::Dot(double x, double y)
{
    this->x = x;
    this->y = y;
}
double Dot::get_x()
{
    return this->x;
}
double Dot::get_y()
{
    return this->y;
}
double Dot::distance(Dot dot1, Dot dot2)
{
    return sqrt(pow(dot1.get_x() - dot2.get_x(), 2) + pow(dot1.get_y() - dot2.get_y(), 2));
}
std::string Dot::to_string()
{
    return "(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ")";
}
bool Dot::operator==(const Dot& dot)
{
    return (this->x == dot.x && this->y == dot.y);
}