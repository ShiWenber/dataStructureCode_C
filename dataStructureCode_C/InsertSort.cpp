/*
    2022.02.22.19:14
    author:cai gou ber
    content:直接插入排序
    1.查找应该插入的位置
    2.将该位置空出，所有已排序元素后移
    3.插入新元素

    这里使用了哨兵，故元素下标从1开始，而不是0
*/


#include <stdio.h>
#include <stdlib.h>

typedef int KeyType;
void InsertSort(KeyType *A, int size){
    // 第一个默认已经排好序
    int i = 0, j = 0; // i是待排序元素下标， j用来遍历已排序元素的下标
    for(int i = 2; i <= size; i++){
        if(A[i - 1] > A[i]){
            A[0] = A[i]; // 存入哨兵, 也起到暂存数据，防止后续操作覆盖掉该数据
            for(j = i - 1; A[j] > A[0]; j--){
                A[j + 1] = A[j]; // 元素后移
            }
            // 此处表示A[j] <= A[0] < A[j+1], 找到插入位置j+1
            A[j + 1] = A[0]; // 插入新元素
        }
        for(int k = 1; k <= size; k++){
           printf("%d ", A[k]);
        }
        printf("\n");
        // 此处表示A[i - 1] <= A[i],不用排序,直接继续下一次最外层循环
    }
}

int main(){
    int size = 0;
    printf("请输入待排序数组的元素个数：");
    scanf("%d", &size);
    KeyType *A = (KeyType *)malloc(sizeof(KeyType) * (size+1));
    printf("请输入待排序数组：\n");
    for(int i = 1; i <= size; i++){
        scanf("%d", &A[i]);
    }
    InsertSort(A, size);
    printf("排序后的数组：\n");
    for(int i = 1; i <= size; i++){
        printf("%d ", A[i]);
    }
    printf("\n");
    return 0;
}