#include <stdio.h>
#include <stdlib.h>
typedef int ElemType;
typedef struct{
    int i,j;
    ElemType data;
}Triple;//三元组
typedef struct{
    Triple *TripleList;
    int row,column,tripleNum;
}SqTSMatrix;
//顺序结构的三元组需要先确定三元组的长度，如果是链式机构的三元组就可以变长,从实用的角度来说用链表更方便使用
int CreateSqTSMatrix(SqTSMatrix &M, int row, int column, int tripleNum){
    //输入三元组
    M.TripleList = (Triple *)malloc(tripleNum*sizeof(Triple));
    for(int i = 0; i < M.tripleNum; i++){
        scanf("%d %d %d",&M.TripleList[i].i,&M.TripleList[i].j,&M.TripleList[i].data);
    }
    M.row = row;
    M.column = column;
    M.tripleNum = tripleNum;
    return 1;
}
int DisplaySqTSMatrix(SqTSMatrix M){
    for()
}
int DestroySqTSMatrix(SqTSMatrix &M){
    free(M.TripleList);
    M.column = 0;
    M.row = 0;
    M.tripleNum = 0;
    return 1;
}
int main(){
    
    return 0;
}