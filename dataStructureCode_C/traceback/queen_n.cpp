#include <chrono>
#include <iostream>

using namespace std;
using namespace chrono;

bool place(int k, int* x) {
  int i = 1;
  while (i < k) {
    if (x[i] == x[k] || abs(k - i) == abs(x[k] - x[i])) {
      return false;
    }
    i += 1;
  }
  return true;
}

/**
 * @brief
 * procedure backtracking(n, i) // n表示维度，i表示当前行
begin
  if i = n then
    print(x); // 输出解
    return
  for j = 1 to n do
    // 处理当前结点
    x[i] = j
    if Place(i, j) then
      backtracking(n, i+1); // 递归
    else
      // 回溯，撤销当前结点的处理结果
      x[i] = 0
      return
end
 * 递归方式，伪代码有误
 *
 * @param n
 */
void queen_n(int n, int i, int* x) {
  if (i > n) {
    for (int j = 1; j <= n; j++) {
      cout << x[j] << " ";
    }
    cout << endl;
    return;
  }
  int j = 1;
  for (j = 1; j <= n; j++) {
    x[i] = j;
    if (place(i, x)) {
      queen_n(n, i + 1, x);
    } else {
      continue;
    }
  }
  // j == n + 1，超过了有效区间，没有找到合适的解，回溯
  x[i] = 0;
  return;
}

void queen_n(int n) {
  //   int* x = new int[n + 1];  // 1空出来
  //   int i = 1;
  //   x[i] = 1;
  //   while (i >= 1 && i <= n) {
  //     while (x[i] <= n && place(i, x) == false) {
  //       x[i]++;
  //     }
  //     // 如果x[i] 仍然处于有效区间则表明通过了place的检验
  //     if (x[i] <= n) {
  //       // 到达叶子节点
  //       if (i == n) {
  //         for (int j = 1; j <= n; j++) {
  //           cout << x[j] << " ";
  //         }
  //         cout << endl;
  //         x[i] ++; // 探索同层下一个节点
  //       } else {
  //         // 未到达叶子节点，继续深入
  //         i++;
  //         x[i] = 1;
  //       }
  //     } else {
  //       // 不存在合法的x[i]或者以及获得了一个解，回溯
  //       // 回溯 并探索同层的下一个节点
  //       i--;
  //       x[i]++;
  //     }
  //   }
  int* x = new int[n + 1];
  int i = 1;
  x[i] = 0;
  while (i >= 1 && i <= n) {
    x[i]++;  // 探索同层下一个节点
    while (x[i] <= n && place(i, x) == false) {
      x[i]++;
    }
    // 如果x[i] 仍然处于有效区间则表明通过了place的检验
    if (x[i] <= n) {
      // 到达叶子节点
      if (i == n) {
        for (int j = 1; j <= n; j++) {
          cout << x[j] << " ";
        }
        cout << endl;
      } else {
        // 未到达叶子节点，继续深入
        i++;
        x[i] = 0;
      }
    } else {
      // 不存在合法的x[i]或者以及获得了一个解，回溯
      i--;
    }
  }
}

int main() {
  // 测试 place
  int* test = new int[5];
  test[1] = 1;
  test[2] = 4;
  test[3] = 2;
  test[4] = 3;
  test[5] = 5;
  cout << place(3, test) << endl;
  cout << place(4, test) << endl;
  // 测试 queen_n
  system_clock::time_point start = system_clock::now();
  queen_n(4);
  system_clock::time_point end = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end - start).count() / 1e6
       << "s" << endl;
  cout << endl;

  system_clock::time_point start1 = system_clock::now();
  queen_n(5);
  system_clock::time_point end1 = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end1 - start1).count() / 1e6
       << "s" << endl;

  system_clock::time_point start2 = system_clock::now();
  queen_n(6);
  system_clock::time_point end2 = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end2 - start2).count() / 1e6
       << "s" << endl;
  cout << endl;
  // 比较从 7 - 16皇后的时间
  system_clock::time_point start3 = system_clock::now();
  queen_n(7);
  system_clock::time_point end3 = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end3 - start3).count() / 1e6
       << "s" << endl;
  cout << endl;

  system_clock::time_point start4 = system_clock::now();
  queen_n(8);
  system_clock::time_point end4 = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end4 - start4).count() / 1e6
       << "s" << endl;
  cout << endl;

  // system_clock::time_point start5 = system_clock::now();
  // queen_n(9);
  // system_clock::time_point end5 = system_clock::now();
  // cout << "time: " << duration_cast<microseconds>(end5 - start5).count() / 1e6
  //      << "s" << endl;
  // cout << endl;

  // system_clock::time_point start6 = system_clock::now();
  // queen_n(10);
  // system_clock::time_point end6 = system_clock::now();
  // cout << "time: " << duration_cast<microseconds>(end6 - start6).count() / 1e6
  //      << "s" << endl;
  // cout << endl;

  // system_clock::time_point start7 = system_clock::now();
  // queen_n(11);
  // system_clock::time_point end7 = system_clock::now();
  // cout << "time: " << duration_cast<microseconds>(end7 - start7).count() / 1e6
  //      << "s" << endl;
  // cout << endl;

  // system_clock::time_point start8 = system_clock::now();
  // queen_n(12);
  // system_clock::time_point end8 = system_clock::now();
  // cout << "time: " << duration_cast<microseconds>(end8 - start8).count() / 1e6
  //      << "s" << endl;
  // cout << endl;

  // system_clock::time_point start9 = system_clock::now();
  // queen_n(13);
  // system_clock::time_point end9 = system_clock::now();
  // cout << "time: " << duration_cast<microseconds>(end9 - start9).count() / 1e6
  //      << "s" << endl;
  // cout << endl;

  // system_clock::time_point start10 = system_clock::now();
  // queen_n(16);
  // system_clock::time_point end10 = system_clock::now();
  // cout << "time: " << duration_cast<microseconds>(end10 - start10).count() /
  // 1e6
  //      << "s" << endl;
  // cout << endl;

  cout << "-------recursive-------" << endl;

  int* x = new int[4 + 1];
  system_clock::time_point start11 = system_clock::now();
  queen_n(4, 1, x);  // 因为从1开始
  system_clock::time_point end11 = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end11 - start11).count() / 1e6
       << "s" << endl;
  cout << endl;

  int *x2 = new int[8 + 1];
  system_clock::time_point start13 = system_clock::now();
  queen_n(8, 1, x2);  // 因为从1开始
  system_clock::time_point end13 = system_clock::now();
  cout << "time: " << duration_cast<microseconds>(end13 - start13).count() / 1e6
       << "s" << endl;
  cout << endl;

  return 0;
}