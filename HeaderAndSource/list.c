#include <stdio.h>
#include <stdlib.h>
typedef int ElemType;
//用LNode类型表示结点，LinkList类型表示头指针/表头
typedef struct LNode{
	ElemType data;
	struct LNode * next;
}LNode,* LinkList;
LinkList find(LinkList &L,int n);
int CreateListFromHead (LinkList & LinkList , int n);
int CreateListFromTail (LinkList & LinkList , int n);
int ListInsert(LinkList & LinkList , int i , ElemType e);
int ListDelete(LinkList & LinkList , int i);
int GetElem(LinkList LinkList , int i , ElemType & e);
int DisplayList (LinkList L);
int main(){
	LinkList list;
	CreateListFromTail(list,5);
//	CreateListFromHead(list,5);
//	ListInsert(list,3,114514);
//	ListDelete(list,3);
//	int a;
//	a = GetElem(list,3,a);
//	printf("%d",a);
	DisplayList(list);
	return 0;
}
int CreateListFromHead (LinkList & L, int n){
	L=(LNode*)malloc(sizeof(LNode));
	L->next=NULL;
	for(int i=0;i<n;i++){
		LinkList p=(LinkList)malloc(sizeof(LinkList));
		scanf("%d",&p->data);
		p->next=L->next;
		L->next=p;
	}
	return 1;
}
int CreateListFromTail (LinkList & L , int n){
	L=(LNode*)malloc(sizeof(LNode));
	L->next=NULL;
	LinkList tail=L;
	for(int i=0;i<n;i++){
		LinkList p=(LinkList)malloc(sizeof(LinkList));
		scanf("%d",&p->data);
		p->next=NULL;
		tail->next=p;
		tail=p;
	}
	return 1;
}
int DisplayList (LinkList L){
	if(!L->next){
		printf("error");
	}else{
		LinkList p=L->next;
		while(p){
			printf("%d ",p->data);
			p=p->next;
		}
	}
	return 1;
}
LinkList find(LinkList &L,int n){
	LinkList p;
	p=L;
	int i=0;
	if(!p->next){
		printf("error");
		return NULL;
	}else{
	while(p->next&&i<n){
		p=p->next;
		i++;
	}
	return p;
	}
}
int ListInsert(LinkList & L , int i , ElemType e){
	LNode *p=(LNode*)malloc(sizeof(LNode));
	LinkList pre=find(L,i);
	p->data=e;
	p->next=pre->next;
	pre->next=p;
	return 1;
}
int ListDelete(LinkList & L , int i){
	LinkList p=find(L,i-1);
	LinkList p1=find(L,i);
	if(p){
		p->next=p->next->next;
		free(p1);
		return 1;
	}else{
		return 0;
	}
}
int GetElem(LinkList L , int i , ElemType & e){
	LinkList p=find(L,i);
	if(p){
		e=p->data;
		return e;
	}else{
		return 0;
	}
}
