#include <stdio.h>
#include <stdlib.h>
typedef int ElemType;
typedef struct LNode
{
    ElemType data;
    LNode * next;
}LNode;
typedef struct LinkStack
{
    LNode * bottom; //作为栈底部
    LNode * top; //作为栈的栈顶指针,也是链的链表头
    int num;//存储的元素个数
}LinkStack;
int InitLinkStack(LinkStack & S){
    S.top = (LNode*)malloc(sizeof(LNode));
    S.top->next = NULL;
    S.bottom = S.top;
    S.num = 0;
    return 1;
}
int LinkStackEmpty(LinkStack  S){
    if(S.bottom == S.top){ //S.num == 0 
        printf("栈空\n");
        return 1;
    }else{
        return 0;
    }
}
//链栈不需要判断栈满的函数，因为动态增长，不需要判断栈满

int GetTop(LinkStack S  , ElemType & e){
    if(LinkStackEmpty(S)){
        return 0;
    }
    e = S.top->data;
    return 1;
}

int Push(LinkStack & S , ElemType e){
    S.top->data = e;
    LNode * pt = (LNode*)malloc(sizeof(LNode));
    pt->next = S.top;
    //插入完成，改变top指向
    S.top = pt;
    //修改top完成
    S.num++;
    return 1;
}
int Pop(LinkStack & S ){
    LNode * temp = S.top;
    S.top = S.top->next;
    free(temp);
    S.num--;
    return 1;
}
//从栈顶到栈底输出栈内已存的数值
int DisplayLinkStack(LinkStack S){
    if(LinkStackEmpty(S)){
        return 0;
    }
    LNode * temp = S.top->next; //跟顺序表同理，将top指针下移一位
    while(temp){
        printf("%d ",temp->data);
        temp = temp->next;
    }
    printf("\n");
    return 1;
}

int main(){
    LinkStack S;
    InitLinkStack(S);
    DisplayLinkStack(S);
    Push(S , 3);
    Push(S , 2);
    Push(S , 1);
    DisplayLinkStack(S);
    Pop(S);
    DisplayLinkStack(S);
    return 0;
}