# 说明

1. 头文件.h后缀，一般只写声明信息，实现需要放在.cpp文件中
2. 对于a.h头文件的实现文件a.cpp，加#include "a.h"
3. 需要使用a.cpp中的代码，就引入#include "a.h"

```powershell
g++ *.cpp -o main.exe
```

注意：

1. g++执行的目录下只能有一个main函数

## g++ 编译器安装

msys2 下安装gnu工具包

[msys2](https://www.msys2.org/)

## 记录运行时间

```c++
// 精度较高
#include <chrono>
using namespace chrono;

system_clock::time_point  start = system_clock::now();
// 算法
system_clock::time_point end = system_clock::now();
cout << "time: " << duration_cast<microseconds>(start - end).count() / 1e6 << "s" << endl;
```