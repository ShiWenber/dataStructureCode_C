// 采用回溯法设计16皇后的程序，输出其中的一个解，并输出其总共有多少个解，需要给出运行时间

#include <chrono>
#include <iostream>

using namespace std;
using namespace chrono;


/**
 * @brief 检查在第row行，第col列放置皇后是否合法
 *
 * @param row 欲放置皇后的行号
 * @param col 欲放置皇后的列号
 * @param queens 记录每一行皇后所在的列号的数组
 * @return true 可以放置
 * @return false 无法放置
 */
bool isValid(int row, int col, int *queens) {
  for (int i = 0; i < row; i++) {  // 检查同一列是否有皇后
    if (queens[i] == col) return false;
  }
  for (int i = row - 1, j = col - 1; i >= 0 && j >= 0;
       i--, j--) {  // 检查左上方是否有皇后
    if (queens[i] == j) return false;
  }
  for (int i = row - 1, j = col + 1; i >= 0 && j < 16;
       i--, j++) {  // 检查右上方是否有皇后
    if (queens[i] == j) return false;
  }
  return true;  // 如果没有冲突，则合法
}

// 尝试在第row行放置皇后
/**
 * @brief 在第row行放置皇后 
 * 
 * @param row 放置皇后的行号
 * @param count 可行解的个数 
 * @param queens 记录当前已经放置的皇后的位置的数组
 * @param output_queens 记录第一个可行解的数组
 */
void placeQueen(int row, int &count, int *queens, int *output_queens) {
  if (row >= 16) {  // 退出递归的条件：如果已经放完了所有皇后遍历到最后一行，则找到了一组解
    count++;        // 统计解的个数
    if (count == 1) {  // 如果是第一个解，则将其保存到output_queens数组中
      for (int i = 0; i < 16; i++) output_queens[i] = queens[i];
    }
    return;
  }

  for (int col = 0; col < 16; col++) {  // 尝试在当前行的每一列放置皇后
    if (isValid(row, col, queens)) {  // 如果可以在当前位置放置皇后
      queens[row] = col;      // 在当前位置放置皇后
      placeQueen(row + 1, count, queens, output_queens);    // 递归到下一行
      queens[row] = -1;        // 回溯，撤销当前放置的皇后
    }
  }
}

// int main() {
//   int count = 0;
//   int queens[16];
//   int output_queens[16]; // 用来接收一个可行解的数组
//   for (int i = 0; i < 16; i++) queens[i] = -1;  // 初始化数组 -1 表示没有皇后
//   system_clock::time_point start = system_clock::now();
//   placeQueen(0, count, queens, output_queens);  // 从第0行开始放置皇后
//   system_clock::time_point end = system_clock::now();
//   cout << "time: " << duration_cast<microseconds>(end - start).count() / 1e6 << "s" << endl;
//   cout << "solution num: " << count << endl;
//   cout << "one solution:" << endl;
//   for (int i = 0; i < 16; i++) {
//     for (int j = 0; j < 16; j++) {
//       if (output_queens[i] == j)
//         cout << "Q ";
//       else
//         cout << "* ";
//     }
//     cout << endl;
//   }
//   return 0;
// }