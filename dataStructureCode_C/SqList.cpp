#include <stdio.h>
#include <stdlib.h>
typedef int ElemType;
typedef struct SqList{
    ElemType *base;
    int length; // 当前使用的存储长度
    int listSize; // 空间长度
}SqList;
//没有使用应用符号，用的指针，效果相同，只不过需要在外部就创建指针SqList *L，而非SqList L
int InitSqList(SqList &L, int size){
    L.base = (ElemType*)malloc(size*sizeof(ElemType)); 
    if(!L.base) return 0;
    L.listSize = size;
    L.length = 0;
    return 1;
}

int IsSqListEmpty(SqList L){
    if(L.length == 0){
        printf("SqList is empty\n");
        return 1;
    }
    return 0;
}

int IsSqListFull(SqList L){
    if(L.length == L.listSize){
        printf("SqList is full\n");
        return 1;
    }
    return 0;
}
//在第i个元素前插入,对应下标为i-1
int SqListInsert(SqList &L, int i, ElemType e){
    if(IsSqListFull(L))
        return 0;
    if(i < 1 || i > L.length + 1){
        printf("i is out of range\n");
        return 0;
    }
    //前面判断过没满，所以这里一定可以访问到L.base[L.length]
    for(int j = L.length - 1; j >= i - 1; j--){
        L.base[j + 1] = L.base[j];
    }
    L.base[i - 1] = e;
    L.length++;
    return 1;
}

int SqListDeleteByKey(SqList &L, int i, ElemType &e){
    if(IsSqListEmpty(L)){
        printf("SqList is empty\n");
        return 0;
    }
    if(i < 1 || i > L.length){
        printf("i is out of range\n");
        return 0;
    }
    e = L.base[i - 1];
    //要避免越界
    for(int j = i - 1; j < L.length - 1; j++){
        L.base[j] = L.base[j + 1];
    }
    L.length--;
    return 1;
}
int SqListDeleteByValue(SqList &L, ElemType e){
    if(IsSqListEmpty(L)){
        printf("SqList is empty\n");
        return 0;
    }
    //方案一，调用SqListDeleteByKey
    ElemType temp;
    for(int i = 0; i < L.length; i++){
        if(L.base[i] == e){
            SqListDeleteByKey(L, i + 1, temp);
            i--; // 因为删除了一个元素，后面的元素补上来了，所以i要减1，再开始遍历，不然会跳过一个元素
        }
    }
    //方案二，写一个类似SqListDeleteByKey的片段
    // for(int i = 0; i < L.length; i++){
    //     if(L.base[i] == e){
    //         //上界length - 2避免越界
    //         for(int j = i; j <= L.length - 2 ; j++){
    //             L.base[j] = L.base[j + 1]; 
    //         }
    //         L.length--;
    //         i--; // 因为删除了一个元素，后面的元素补上来了，所以i要减1，再开始遍历，不然会跳过一个元素
    //     }else{
    //         continue;
    //     }
    // }
    return 1;
}

int DisplaySqList(SqList L){
    if(IsSqListEmpty(L)){
        printf("SqList is empty\n");
        return 0;
    }
    for(int i = 0; i < L.length; i++){
        printf("%d ", L.base[i]);
    }
    printf("\n");
    return 1;
}
int main(){
    SqList L;
    InitSqList(L, 10);
    for(int i = 0; i < 5; i++){
        SqListInsert(L, i + 1, i);
    }
    SqListInsert(L, 1, 100);
    SqListInsert(L, 4, 100);
    DisplaySqList(L);
    ElemType e;
    SqListDeleteByKey(L, 2, e);
    printf("%d\n", e);
    DisplaySqList(L);
    SqListDeleteByValue(L, 100);
    DisplaySqList(L);
    return 0;
}