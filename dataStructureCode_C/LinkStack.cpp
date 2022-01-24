#include <stdio.h>
#include <stdlib.h>
//单链表实现的栈

typedef int ElemType;
typedef struct LNode
{
    ElemType data;
    LNode * next;
}LNode;
typedef struct LinkStack
{
    LNode * bottom; //链表尾
    LNode * top; //链表头，栈顶
}LinkStack;

int InitLinkStack(LinkStack & S){
    S.top = (LNode *)malloc(sizeof(LNode));
    if(!S.top){
        return 0; //exit(0);
    }
    S.bottom = S.top;
    S.top->next = NULL;
    return 1;
}
int LinkStackEmpty(LinkStack S){
    if(S.top == S.bottom){
        printf("栈空\n");
        return 1;
    }else{
        return 0;
    }
}
//向栈S中压入数据e
int Push(LinkStack & S , ElemType e){
    S.top->data = e;
    LNode * pt = (LNode*)malloc(sizeof(LNode));
    pt->next = S.top;
    S.top = pt;
    return 1;
}
int GetTop(LinkStack S , ElemType & e){
    if(LinkStackEmpty(S)){
        return 0;
    }
    e = S.top->next->data;
    return 1;
}

int Pop(LinkStack & S , ElemType & e){
    if(LinkStackEmpty(S)){
        return 0;
    }
    e = S.top->next->data;
    ////////GetTop(S, e);
    LNode * temp = S.top;
    S.top = S.top->next; 
    free(temp);
    return 1;
}
//从栈顶到栈底的顺序输出栈内存的数据元素
int DisplayLinkStack(LinkStack S ){
    if(LinkStackEmpty(S)){
        return 0;
    }
    LNode * p = S.top->next;
    while(p){
        printf("%d ",p->data);
        p = p->next;
    }
    printf("\n");
    //p已经为NULL
    return 1;
}

int main(){
    LinkStack S ;
    InitLinkStack(S);
    DisplayLinkStack(S);
    Push(S,1);
    Push(S,2);
    Push(S,3);
    DisplayLinkStack(S);
    ElemType out;
    GetTop(S,out);
    printf("%d\n",out);
    Pop(S,out);
    printf("%d\n",out);
    DisplayLinkStack(S);
    return 0;
}