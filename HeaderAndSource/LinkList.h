#ifndef __LINKLIST_H__
#define __LINKLIST_H__

/////////////头文件的编写模板
// #ifndef   LABEL
// #define   LABEL
//  /*代码部分*/
// #endif
//代码意思：如果没有定义LABEL，if not define LABEL
//则定义LABEL且编译代码部分，#endif是#ifndef语句的结束标志。
//当重复引用LABEL表示的头文件时，
//由于已经被定义，
//代码部分就不会被编译，能避免重复定义的报错出现
//////////////////////

typedef int ElemType;
//用LNode类型表示结点，LinkList类型表示头指针/表头
typedef struct LNode{
    ElemType data;
    struct LNode * next;
}LNode,* LinkList;

int CreateListFromHead (LinkList & LinkList , int n);
int CreateListFromTail (LinkList & LinkList , int n);
int ListInsert(LinkList & LinkList , int i , ElemType e);
int ListDelete(LinkList & LinkList , int i);
int GetElem(LinkList LinkList , int i , ElemType & e);
int DisplayList (LinkList L);

#endif