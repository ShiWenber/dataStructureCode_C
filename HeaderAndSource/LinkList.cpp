#include<stdio.h>
#include<stdlib.h>
#ifndef __LINKLIST_H__
#include "LinkList.h"
#endif

//线性链表的基本操作实现

//头插法建立链表,含有一个头结点,除头结点外有n个数据结点
int CreateListFromHead(LinkList & L , int n){
    L = (LNode *)malloc(sizeof(LNode));
    L->next = NULL;
    //头结点建立完成
    for (int i = 0; i < n; i++)
    {
        LNode * p = (LNode*)malloc(sizeof(LNode)); //新建一个结点，并用pt指针指向它
        scanf("%d",&p->data);
        p->next = L->next;
        L->next = p;
    }
    return 1;
}
//尾插法建立链表，含有一个头结点，除头结点之外有n个数据结点
int CreateListFromTail(LinkList & L , int n){
    L = (LNode *)malloc(sizeof(LNode));
    L->next = NULL;
    LNode * tail = L;
    for (int i = 0; i < n; i++)
    {
        LNode * pt = (LNode*)malloc(sizeof(LNode));
        scanf("%d",&pt->data);
        pt->next = NULL; //pt->next = tail->next;
        tail->next = pt;
        tail = pt;
    }
    return 1;
}

int DisplayList(LinkList L){
    if(!L->next){
        printf("空表\n");
        return 0;
    }else{
        LNode * p = L->next; //头结点不打印
        while(p){ //当p指向NULL，循环停止
            printf("%d ",p->data);
            p = p->next;
        }
        printf("\n");
        return 1;
    }    
}
//向链表L中第i个位置插入一个数据e
int ListInsert(LinkList & L , int i , ElemType e){
    LNode * pre = L;
    int current = 0;
    while(pre && current < i - 1){
        pre = pre->next;
        current++;
    }
    //pre指NULL/current == i - 1
    if(!pre){
        return 0;
    }
    //前面都可以看成输入的合法性判定
    LNode * pt = (LNode *)malloc(sizeof(LNode));
    pt->data = e;
    pt->next = pre->next;
    pre->next = pt;
    return 1;
}

int ListDelete(LinkList & L , int i){
    LNode * pre = L;
    int current = 0;
    while(pre && current < i - 1){
        pre = pre->next;
        current++;
    }
    if(!pre){
        return 0;
    }
    LNode * temp = pre->next;
    pre->next = pre->next->next;
    free(temp);
    return 1;
}
//获得链表L中第i个结点的数据
int GetElem(LinkList L , int i, ElemType & e){
    LNode * p = L;
    int current = 0;
    while(p && current < i ){
        p = p->next;
        current++;
    }
    if(!p){
        return 0;
    }
    //此处current == i
    e = p->data;
    return 1;
}


// // 测试用主函数
// int main(){
//     LinkList head;
//     LinkList head2;
//     CreateListFromHead(head,5);
//     DisplayList(head);
//     CreateListFromTail(head2 , 6);
//     DisplayList(head2);
//     ListInsert(head , 2 , 6 );
//     DisplayList(head);
//     ListDelete(head , 2);
//     DisplayList(head);
//     ElemType e;
//     GetElem(head , 3 , e);
//     printf("%d\n",e);
//     DisplayList(head);
//     return 0;
// }