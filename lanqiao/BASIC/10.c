#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct LNode{
	int data;
	struct LNode * next;
}LNode , * LinkList;
int display(LinkList hex){
	LNode * p = hex->next;
	while(p){
		if(0 <= p->data && p->data <= 9){
			printf("%c",(char)(p->data) + '0');
		}else{
			printf("%c",(char)(p->data - 10) + 'A'  );
		}
		p = p->next;
	} 
}
int main(){
	int dec = 0;
	int temp,res;
	scanf("%d",&dec);
	temp = dec;
	//建立链表的头
	LinkList hex = (LNode *)malloc(sizeof(LNode)); 
	hex->next = NULL;
	if(temp == 0){
		printf("0");
		return 0; 
	}
	while(temp != 0){
		res = temp%16;
		temp = temp/16;
		//头插法插入链表
		LNode * pt = (LNode *)malloc(sizeof(LNode)); 
		pt->data = res;
		pt->next = hex->next;
		hex->next =  pt;
		//
		
	}
	//若为0则前面的循环不起作用 
	//输出链表元素
	display(hex);
	system("pause");
	return 0;
}