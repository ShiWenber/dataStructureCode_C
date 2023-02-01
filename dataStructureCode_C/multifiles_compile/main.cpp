#include "random_array.h"
#include <iostream> // 等效于C中的iostd.h

// using namespace std;

int main() {
    double *a= new double[10];
    random_array(a, 10);
    std::cout << "hello" << std::endl;
    for (int i = 0; i < 10; i ++) {
        std::cout << a[i] << std::endl;
    }
    // cout << 
    return 0;
}
