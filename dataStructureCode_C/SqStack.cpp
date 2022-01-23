#include <stdio.h>
#include <stdlib.h>
typedef int ElemType;

typedef struct{
    ElemType * base;//空间基址，栈底
    ElemType * top; // 栈顶
    int stacksize;
}SqStack;

//初始化一个最多能存n个数据元素的栈
int initSqStack(SqStack & S , int n){
    S.base = (ElemType *)malloc(n*sizeof(ElemType));
    //分配空间失败
    if(!S.base){
        return 0; //exit(0)
    }
    S.top = S.base;
    S.stacksize = n;
    return 1;
}

int SqStackFull(SqStack & S){
    if(S.top - S.base == S.stacksize){
        printf("栈满\n");
        return 1;
    }else{
        return 0;
    }
}
int SqStackEmpty(SqStack S){
    if(S.base == S.top){
        printf("栈空\n");
        return 1;
    }else{
        return 0;
    }
}
int GetTop(SqStack  S , ElemType & out){
    if(SqStackEmpty(S)){
        return 0;
    }
    out = *(S.top-1);
    return 1;
}
//向栈S中压数据e
int Push(SqStack & S , ElemType e){
    if(SqStackFull(S)){
        return 0;
    }
    *S.top++  = e; // *S.top = e ; S.top++;
    return 1;
}
int Pop(SqStack & S , ElemType & out){
    if(SqStackEmpty(S)){
        return 0;
    }
    out = *(--S.top); //out = *(S.TOP - 1 ); S.top --;
    return 0;
}
int DisplaySqStack(SqStack S){
    for(int i =0;i < (S.top - S.base) ; i++){
        printf("%d\n",S.base[i]);
    }
    return 1;
}

int main(){
    SqStack S;
    initSqStack(S,4);
    Push(S , 1);
    Push(S,2);
    Push(S,3);
    DisplaySqStack(S);
    ElemType out;
    Pop(S,out);
    printf("%d\n",out);
    DisplaySqStack(S);
    return 0;
}