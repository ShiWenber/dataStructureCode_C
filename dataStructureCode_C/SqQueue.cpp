#include <stdio.h>
#include <stdlib.h>


typedef int ElemType;
typedef struct SqQueue
{
    ElemType *base;
    int front;
    int rear;
    int maxsize;
    int tag;  // 当舍弃一个空间时无用，用来标记当头尾相同时是否为空
}SqQueue;

int InitQueue(SqQueue &Q, int maxsize);
int SqQueueEmpty(SqQueue Q);
int SqQueueFull(SqQueue Q);
int EnSqQueue(SqQueue &Q, ElemType e);
int OutSqQueue(SqQueue &Q, ElemType &e);
int SqQueueLength(SqQueue Q);
int Display(SqQueue Q);


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
    SqQueue Q;
    InitQueue(Q,5);
    EnSqQueue(Q,1);
    EnSqQueue(Q,2);
    EnSqQueue(Q,3);
    EnSqQueue(Q,4);
    EnSqQueue(Q,5);
    Display(Q);
    printf("%d\n",SqQueueLength(Q));
    ElemType e;
    OutSqQueue(Q,e);
    printf("%d\n",e);
    OutSqQueue(Q,e);
    printf("%d\n",e);
    OutSqQueue(Q,e);
    printf("%d\n",e);
    OutSqQueue(Q,e);
    printf("%d\n",e);
    OutSqQueue(Q,e);
    printf("%d\n",e);
    Display(Q);
    printf("%d\n", e);
    EnSqQueue(Q,6);
    Display(Q);
    printf("%d,%d\n", Q.front, Q.rear);
    return 0;
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


// //不舍弃一个空间，使用tag来标记队列是否为空
// int InitQueue(SqQueue &Q, int maxsize){
//     Q.base = (ElemType *)malloc(maxsize * sizeof(ElemType));
//     if(!Q.base){
//         printf("malloc error!\n");
//         return 0;
//     }
//     Q.front = 0;
//     Q.rear = Q.front;
//     Q.maxsize = maxsize;
//     Q.tag = 1;
//     return 1;
// }

// int SqQueueEmpty(SqQueue Q){
//     if(Q.front == Q.rear && Q.tag){ // 队空
//         printf("Queue Empty\n");
//         return 1;
//     }
//     return 0;
// }

// int SqQueueFull(SqQueue Q){
//     if(Q.front == Q.rear && !Q.tag){ // 队满
//         printf("Queue Full\n");
//         return 1;
//     }
//     return 0;
// }

// int EnSqQueue(SqQueue &Q, ElemType e){
//     if(SqQueueFull(Q)){
//         return 0;
//     }
//     Q.base[Q.rear] = e;
//     Q.rear = (Q.rear + 1) % Q.maxsize;
//     if(Q.front == Q.rear){
//         Q.tag = 0; // 表示队满
//     }
//     return 1;
// }

// int OutSqQueue(SqQueue &Q, ElemType &e){
//     if(SqQueueEmpty(Q)){
//         return 0;
//     }
//     e = Q.base[Q.front];
//     Q.front = (Q.front + 1) % Q.maxsize;
//     if(Q.front == Q.rear){
//         Q.tag = 1; // 表示队空
//     }
//     return 0;
// }
