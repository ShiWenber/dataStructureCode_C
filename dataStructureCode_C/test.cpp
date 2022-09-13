#include <stdio.h>

int BubbleSort(int * a, int len){
    int temp;
    if(len <= 0)
        return 0;
    for (int i = 0; i < len - 1; i++)
    {
        for (int j = 0; j < len - i - 1; j++)
        {
            if(a[j] > a[j + 1]){
                temp = a[j];
                a[j] = a[j+1];
                a[j+1] = temp;
                // for (int k = 0; k < len; k++)
                // {
                //     printf("%d ",a[k]);
                // }
                // printf("\n");
            }
        }
        // for (int k = 0; k < len; k++)
        // {
        //     printf("-%d ",a[k]);
        // }
        // printf("\n");
    }
    return 1;
}

int main(){
    int a[100];
    int i = 0;
    while(scanf("%d",&a[i]) != EOF){ 
        i++;
    }
    // while(~scanf("%d",&a[i])){ 
    //     i++;
    // }
    for (int k = 0; k < i; k++)
    {
        printf("%d ",a[k]);
    }
    BubbleSort(a,i);
    printf("\n");
    for (int k = 0; k < i; k++)
    {
        printf("%d ",a[k]);
    }
    
}