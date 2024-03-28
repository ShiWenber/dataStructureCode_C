/**
 * @file Dot.h
 * @author your name (you@domain.com)
 * @brief 点类 
 * @version 0.1
 * @date 2022-10-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DOT_H
#define DOT_H
/**
 * @brief 点对象 
 * 
 */
class Dot {
    private:
        double x;
        double y;
    public:
        Dot(){}; // 默认构造函数
        Dot(double x, double y);
        double get_x();
        double get_y();
        static double distance(Dot dot1, Dot dot2);
        std::string to_string();
        bool operator==(const Dot& dot);
};
#endif