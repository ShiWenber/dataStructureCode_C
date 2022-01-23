# include<stdio.h>
# include<string.h>
# include<stdlib.h>
int toNum(char input)
{   
    return input - '0'; 
}
char toChar(int input)
{
    return input + '0'; 
}
int main(){
    //求五位十进制数
    int i=0,j=0,k=0,m=0,n=0;
    int str[5] = {0};
    int str2[6] = {0};  
    scanf("%d",&n);
    for ( i = 1; i <= 9; i++)
    {
        for ( j = 0; j <= 9; j++)
        {
            for ( k = 0; k <= 9; k++)
            {
                if((2*i + 2*j + k)  == n)
                {
                    str[0] = i;
                    str[1] = j;
                    str[2] = k;
                    str[3] = j;
                    str[4] = i;
                    for(m = 0; m < 5 ; m++)
                        printf("%d",str[m]);
                    printf("\n");
                }
            }
            
        }
        
    }
    
    //求六位十进制数
    for ( i = 1; i <= 9; i++)
    {
        for ( j = 0; j <= 9; j++)
        {
            for ( k = 0; k <= 9; k++)
            {
                    if(2*(i+j+k)==n)
                    {
                        str2[0] = i;
                        str2[1] = j;
                        str2[2] = k;
                        str2[3] = k;
                        str2[4] = j;
                        str2[5] = i;
                        for(m = 0; m < 6 ; m++)
                            printf("%d",str2[m]);
                        printf("\n");
                    }
            }
            
        }
        
    }
    
    return 0;
}