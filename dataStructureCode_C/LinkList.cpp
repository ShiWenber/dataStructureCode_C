# include<stdio.h>
# include<stdlib.h>

typedef int ElemType;
//用LNode类型表示结点，LinkList类型表示头指针/表头
typedef struct LNode{
    ElemType data;
    struct LNode * next;
}LNode,* LinkList;

//使用头插法以L为头指针建立一个链表，除开头结点有n个数据结点
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

int main(){
    LinkList head;
    CreateListFromHead(head,5);
    DisplayList(head);
    return 0;
}
