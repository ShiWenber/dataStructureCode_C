/**
 * @file bag01.cpp
 * @author your name (you@domain.com)
 * @brief 01背包问题 
 * @version 0.1
 * @date 2022-11-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <fstream>


using namespace std;


/**
 * @brief 01背包问题 
 * 
 * @param n 物品个数
 * @param s 物品体积数组
 * @param v 物品价值数组
 * @param bag_c 背包容量
 * @return int 
 */
int bag01_dp(int n, int *s, int *v, int bag_c) {
    int **dp = new int *[n + 1];
    for (int i = 0; i <= n; i++) {
        dp[i] = new int[bag_c + 1];
    }
    for (int i = 0; i <= n; i++) {
        dp[i][0] = 0;
    }
    for (int i = 0; i <= bag_c; i++) {
        dp[0][i] = 0;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= bag_c; j++) {
        if (j < s[i - 1]) {
            dp[i][j] = dp[i - 1][j];
        } else {
            dp[i][j] = std::max(dp[i - 1][j], dp[i - 1][j - s[i - 1]] + v[i - 1]);
        }
        cout << dp[i][j] << "\t";
        }
        cout << endl;
    }
    // dp存储为csv文件
    ofstream fout("dp.csv");
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= bag_c; j++) {
        fout << dp[i][j] << ",";
        }
        fout << endl;
    }
    fout.close();
    int max = dp[n][bag_c];
    for (int i = 0; i <= n; i++) {
        delete[] dp[i];
    }
    delete[] dp;
    return max;
}

// int main(int argc, char const *argv[])
// {
//     int n = 5;
//     int s[] = {3,5,7,8,9};
//     int v[] = {4,6,7,9,10};
//     int bag_c = 22;
//     bag01_dp(n, s, v, bag_c);
//     return 0;
// }
