#include <iostream>

using namespace std;

int n = 0;
int h[10] = {1};

bool check(int e) {
  for(int i = 1; i <= n; i ++) {
    // e = e * 2 - h[i];
    if ((e*2-h[i]) >= 1e5)
    {
      return true;
    }
    if ((e*2-h[i]) < 0)
    {
      return false;
    }
  }
  return true;
}

int main() {
  cout << check(1) << endl;
  return 0;
}