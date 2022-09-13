// 利用栈将队列逆置
#include <stdio.h>
#include <stdlib.h>


typedef int ElemType;

typedef struct{
    ElemType * base;//空间基址，栈底
    ElemType * top; // 栈顶
    int stacksize; //表示栈具有的空间长度
}SqStack;


typedef struct SqQueue
{
    ElemType *base;
    int front;
    int rear;
    int maxsize;
    int tag;  // 当舍弃一个空间时无用，用来标记当头尾相同时是否为空
}SqQueue;

int InitSqStack(SqStack & S,int n);

//初始化一个最多能存n个数据元素的栈
int InitSqStack(SqStack & S , int n){
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

int InitQueue(SqQueue &Q, int maxsize){
    Q.base = (ElemType *)malloc(maxsize * sizeof(ElemType));
    if(!Q.base){
        printf("malloc error!\n");
        return 0;
    }
    Q.front = 0;
    Q.rear = Q.front;
    Q.maxsize = maxsize;
    return 1;
}

int SqQueueEmpty(SqQueue Q){
    if(Q.front == Q.rear){ // 队空
        printf("Queue Empty\n");
        return 1;
    }
    return 0;
}

int SqQueueFull(SqQueue Q){
    if((Q.rear + 1) % Q.maxsize == Q.front){ // 队满
        printf("Queue Full\n");
        return 1;
    }
    return 0;
}

int EnSqQueue(SqQueue &Q, ElemType e){
    if(SqQueueFull(Q)){
        return 0;
    }
    Q.base[Q.rear] = e;
    Q.rear = (Q.rear + 1) % Q.maxsize;
    return 1;
}

int OutSqQueue(SqQueue &Q, ElemType &e){
    if(SqQueueEmpty(Q)){
        return 0;
    }
    e = Q.base[Q.front];
    Q.front = (Q.front + 1) % Q.maxsize;
    return 1;
}


// ？？如何遍历队列中的元素？？
int Display(SqQueue Q){
    if(SqQueueEmpty(Q)){
        return 0;
    }
    // i只限定次数，下标由index另求
    int index = Q.front;
    int times = SqQueueLength(Q);
    for(int i = 0; i < times; i++, index++){
        index = index % Q.maxsize;
        printf("%d ",Q.base[index]);
    }
    printf("\n");
    return 1;
}


int SqQueueLength(SqQueue Q){
    // 如果使用tag就不必作此if判断
    if(SqQueueFull(Q)){
        return Q.maxsize;
    }
    return ((Q.rear + Q.maxsize - Q.front) % Q.maxsize); 
}

int main(){
    SqStack S;
    SqQueue Q;
    InitQueue(Q,5);
    InitSqStack(S,4);
    EnSqQueue(Q,1);
    EnSqQueue(Q,2);
    EnSqQueue(Q,3);
    EnSqQueue(Q,4);
    EnSqQueue(Q,5);
    Display(Q);
    
    return 0;
}