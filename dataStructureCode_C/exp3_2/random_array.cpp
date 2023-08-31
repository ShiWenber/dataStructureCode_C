/**
 * @file random.cpp
 * @author your name (you@domain.com)
 * @brief 随机数生成，长度为10000的数组
 * @version 0.1
 * @date 2022-10-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <cstdlib>
#include <time.h>
#include "random_array.h"

using namespace std;

void random_array(double*a, int n)
{
  srand((unsigned)time(NULL));
  for (int i = 0; i < n; i++)
  {
    /**生成从-5000到4999的随机数*/
    a[i] = rand() % 10000 - 5000;
  }
}


// int main()
// {
//   double a[10000];
//   random_array(a, 10000);
//   /** 显示最大值和最小值 */
//   int max = a[0], min = a[0];
//   for (int i = 0; i < 10000; i++)
//   {
//     cout << a[i] << " ";
//     if (a[i] > max)
//       max = a[i];
//     if (a[i] < min)
//       min = a[i];
//   }
//   cout << endl;
//   cout << "max = " << max << endl;
//   cout << "min = " << min << endl;
//   return 0;
// }