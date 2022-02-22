#include <stdio.h>
#include <stdlib.h>
// 返回值为插入位置（如果找到，往找到的位置插，如果没找到，返回值就是插入位置），注意传入的参数nums元素下标从1开始,1~numsSize
int BinarySearchForInsert(int* nums, int numsSize, int target){
    int left = 1;
    int right = numsSize;
    int middle = 0;
    int key = 0; // 标记while循环从何种情况退出，没找到退出时一定左边比右边大一，在完成加减一之前一定left == right == middle
    while(left <= right){
        middle = (left + right) / 2;
        if(target == nums[middle]){
            return middle;
        }else if(target > nums[middle]){
            left = middle + 1;
            key = 1;//大于中间值的情况退出循环则在middle加0后插入
        }else{//到此处一定target < nums[middle]
            right = middle - 1;
            key = 0;//小于中间值的情况退出循环则在middle减1后插入
        }
    }
    // 到此处表示没找到，key为1表示退出循环前target在mid位置的右边，key为0表示在mid位置的左边，当返回mid + key的时候（不管有没有找到），mid + key 就表示应该插入的位置
    return middle + key;
}

void BinaryInsertSort(int *A, int n){
    int p; // 存放插入位置
    for(int i = 2; i <= n; i++){
    // 需要一个暂存的哨兵位置，也能当作target用
        A[0] = A[i];  
        p = BinarySearchForInsert(A, i - 1, A[0]);
        for(int j = i; j > p; j-- ){
            A[j] = A[j - 1]; // 后移
        }
        // j 最多到p + 1，刚好A[p + 1] = A[p],将p位置空出来
        A[p] = A[0]; // 插入

    }
}
int main(){
    int A[5] = {0, 5, 2, 4, 1};
    BinaryInsertSort(A, 5);
    for(int i = 1; i <= 4; i++){
        printf("%d ", A[i]);
    }
    return 0;
}