#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    int i ,j;
    for ( i = 1; i < 9; i++)
    {
        for ( j = 0; j < 9; j++)
        {  
            printf("%d%d%d%d\n",i,j,j,i);   
        }
    }
    
    return 0;
}