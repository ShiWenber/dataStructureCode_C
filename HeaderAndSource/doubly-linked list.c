#include <stdio.h>
#include <stdlib.h>
typedef int ElemType;
typedef struct Node {
	ElemType data;
	struct Node *pre;
	struct Node *next;
} Node, *List;
List Lfind(List &L, int n);
List Lcreateh(int n);
List Lcreatet(int n);
int Linserct(List &L, int n, ElemType e);
int Ldelect(List &L, int n);
int Ldisplay(List &L);
ElemType Lget(List &L, int i);
int main() {
//	List l =Lcreateh(5);
	List l = Lcreatet(5);
//	printf("%d\n",Lget(l,3));
//	Linserct(l,3,114514);
//	Ldelect(l,3);
	Ldisplay(l);
	return 0;
}
List Lcreateh(int n) {
	Node *l = (Node *)malloc(sizeof(Node));
	l->next = NULL;
	l->pre = NULL;

	for(int i = 0; i < n; i++) {
		List p = (List)malloc(sizeof(List));
		scanf("%d", &p->data);
		p->next = l->next;
		p->pre = l;
		l->next = p;
	}

	return l;
}
List Lcreatet(int n) {
	Node *l = (Node *)malloc(sizeof(Node));
	l->next = NULL;
	l->pre = NULL;
	List tail = l;

	for(int i = 0; i < n; i++) {
		List p = (List)malloc(sizeof(List));
		scanf("%d", &p->data);
		p->pre = tail;
		p->next = tail->next;
		tail->next = p;
		tail = p;
	}

	return l;
}
int Ldisplay(List &L) {
	if(!L) {
		printf("error");
		return 0;
	} else {
		List p = L->next;

		while(p) {
			printf("%d ", p->data);
			p = p->next;
		}

		return 1;
	}
}
List Lfind(List &L, int n) {
	List p = L;

	if(!p) {
		return NULL;
	} else {
		int i = 0;

		while(p && i < n) {
			p = p->next;
			i++;
		}

		return p;
	}
}
ElemType Lget(List &L, int i) {
	ElemType e = Lfind(L, i)->data;
	return e;
}
int Lnserct(List &L, int n, ElemType e) {
	Node *p = (Node *)malloc(sizeof(Node));
	List l = Lfind(L, n);
	p->data = e;
	p->pre = l;
	p->next = l->next;
	l->next->pre = p;
	l->next = p;
	return 1;
}
int Ldelect(List &L, int n) {
	List p = Lfind(L, n);

	if(p) {
		p->pre->next = p->next;
		p->next->pre = p->pre;
		free(p);
		return 1;
	} else {
		return 0;
	}
}
