#include <stdio.h>
#include <stdlib.h>
typedef int ElemType;
typedef struct QNode
{
    ElemType data;
    QNode * next;
}QNode;
typedef struct LinkQueue
{
    QNode * front; //队头指针，链表的头指针
    QNode * rear; //队尾指针
    //尾入头出
}LinkQueue;
int InitLinkQueue(LinkQueue & Q){
    Q.front = (QNode *)malloc(sizeof(QNode));// ????Q = (LinkQueue)malloc???
    if(!Q.front){
        return 0; //exit(0)
    }
    Q.front->next = NULL;
    Q.rear = Q.front; // 队列空的条件
    return 1;
}
int EnQueue(LinkQueue & Q , ElemType e){
    QNode * p = (QNode *)malloc(sizeof(QNode));
    p->data = e;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
    return 1;
}
//判断队列是否为空
int LinkQueueEmpty(LinkQueue Q){
    if(Q.front == Q.rear){
        printf("队列空\n");
        return 1;
    }else{
        return 0;
    }
}
//获得队头元素
int GetHead(LinkQueue Q , ElemType & e){
    if(LinkQueueEmpty(Q)){
        return 0;
    }
    e = Q.front->next->data;
    return 1;
}
//出队
int DeLinkQueue(LinkQueue & Q , ElemType & e){
    GetHead(Q , e);
    QNode * temp = Q.front->next;
    Q.front->next = Q.front->next->next;
    free(temp);
    return 1;
}
//展示队列当前存的内容
int DisplayLinkQueue(LinkQueue Q){
    if(LinkQueueEmpty(Q)){
        return 0;
    }
    QNode * current = Q.front->next; 
    while(current){
        printf("%d " , current->data);
        current = current->next;
    }
    printf("\n");
    return 1;
}

//单链队列
int main(){
    LinkQueue Q;
    InitLinkQueue(Q);
    DisplayLinkQueue(Q);
    EnQueue(Q , 3);
    EnQueue(Q , 4);
    EnQueue(Q , 5);
    DisplayLinkQueue(Q);
    ElemType e;
    GetHead(Q , e);
    printf("%d\n",e);
    DeLinkQueue(Q , e);
    printf("%d\n",e);
    DisplayLinkQueue(Q);
    return 0;
}







































// #include <string.h>
// typedef int ElemType;
// typedef struct LNode
// {
//     ElemType data;
//     struct LNode * next;
// }LNode , * QueuePt;
// typedef struct LinkQueue
// {
//     QueuePt front;
//     QueuePt rear;
// }LinkQueue;
// //该链式队列尾入头出
// int InitLinkQueue(LinkQueue & Q){
//     Q.front = (LNode *)malloc(sizeof(LNode));
//     Q.front->next = NULL;
//     Q.rear = Q.front;
// }
// int LinkQueueEmpty(LinkQueue Q){
//     if(Q.front == Q.rear){
//         printf("队空\n");
//         return 1;
//     }else{
//         return 0;
//     }
// }
// int Dequeue(LinkQueue & Q){
//     if(LinkQueueEmpty(Q)){
//         return 0;
//     }
// }
// int Enqueue(LinkQueue & Q){
//     LNode * pt = (LNode*)malloc(sizeof(LNode));
//     LNode 
// }
// int main(){
    
//     return 0;
// }