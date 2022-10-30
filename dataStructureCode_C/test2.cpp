#include <iostream>

using namespace std;
const int N = 100010;
int h[N];
int n;

bool check(int e)
{
    for(int i = 1 ; i <= n ; i ++)
    {
        e = e * 2 - h[i];
        if(e >= 1e5)
        {
            return true;
        }
        if(e < 0)   //注意e不可等于0，比如走到最后一个数的时候刚好等0也符合题意
        {
            return false;
        }
    }
    return true;
}
int main()
{
    scanf("%d",&n);
    for(int i = 1 ; i <= n ; i++)
    {
        scanf("%d",&h[i]);
    }
    /**r取n能减少while次数*/
    int l = 0 ,r = n;
    while(l < r)
    {
        int mid = l + r >> 1;
        if(check(mid))
        {
            r = mid;
        }
        else
        {
            l = mid + 1;
        }
    }
    printf("%d\n",l);
    return  0;
}
