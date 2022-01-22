#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//求n次方
long long a_n( long long a , long long n){
	int i = 0;
	long long sum = 1;
	for(i = 0 ; i < n ; i++){
		sum*=a;
	}	
	return sum;
}

//字符转数字
int toNum(char in ){
	if('0'<= in && in <= '9'){
		return in - '0';
	}else{
		return in - 'A' + 10;
	}
} 
int main(){
	char hex[8];
	scanf("%s",&hex);
	//printf("%s",hex);
	long long dec[8]={0};
	int len = strlen(hex);
	int i , k, t;
	//printf("%d",strlen(hex));
	for( i = len-1,t = 1 ; i >= 0 ; i--,t++ )
	{ 
		k = toNum(hex[i]);
		dec[i] = k * a_n( 16 , t - 1 );
	}
	long long sum=0;
	for(i = 0;i < len;i++){
		sum += dec[i]; 
	}
	printf("%lld\n" , sum);
	return 0;
} 