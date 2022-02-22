#include "BiTree.cpp"

BiTreeNode * BST_Search(BiTree T, ElemType key){
    while (T!=NULL && T->data != key)
    {
        if(key < T->data){
            T = T->lchild;
        }else{
            T = T->rchild;
        }
    }
    // 情况为while的反面，即T为空或者T->data == key
    return T;
}
// 非递归的插入方法, 有严重问题，先不要使用???????
int BST_Insert(BiTree &T, ElemType key){
    BiTree p = T; // 由于会对T进行修改，所以需要用p来保存T的地址
    if(p == NULL){
        p = (BiTreeNode *)malloc(sizeof(BiTreeNode));
        p->data = key;
        p->lchild = p->rchild = NULL;
        // T初始化完成
        return 1;
    }
    while(p != NULL && key != p->data){
        if(key < p->data){
            // 左子树为空，则插入
            if(p->lchild == NULL){
                p->lchild = (BiTreeNode *)malloc(sizeof(BiTreeNode));
                p->lchild->data = key;
                p->lchild->lchild = p->lchild->rchild = NULL;
                printf("insert %d to left of %d\n", key, p->data);
                return 1;
            }
            p = p->lchild;
        }else{
            if(p->rchild == NULL){
                p->rchild = (BiTreeNode *)malloc(sizeof(BiTreeNode));
                p->rchild->data = key;
                p->rchild->lchild = p->rchild->rchild = NULL;
                printf("insert %d to right of %d\n", key, p->data);
                return 1;
            }
            p = p->rchild;
        }
    }
    return 0;
    // 不能到这里等p为空再做修改，会导致分配的空间没有链到树上
    // 易错点，递归方法和非递归方法的差别，递归中由于&传递的是指针的引用，因此新建叶子结点链在树上，但是非递归中由于传递的是指针的NULL值，因此新建叶子结点游离
}



// 递归模板
// int function(T){
//     分类讨论，确定递归还是做其他处理（注意在其他处理中一定要有return等退出当前递归层次的语句，退出条件就是写有该语句的情况）
// }
// 递归的插入方法，由于最终递归退回到最浅层，所以不像非递归需要考虑是否要暂存T的值（递归函数中似乎无法暂存）
int BST_Insert_recursive(BiTree &T, ElemType key){
    if(T == NULL){
        T = (BiTreeNode *)malloc(sizeof(BiTreeNode));
        T->data = key;
        T->lchild = T->rchild = NULL;
        return 1;
    }else if(key < T->data){
        return BST_Insert_recursive(T->lchild, key); // 如果key小于根节点就往左子树插入, 插入完了需要退出当前层次，需要添加return，这里函数返回int（当bool用）可以直接在BST前加上return
    }else if(key > T->data){
        return BST_Insert_recursive(T->rchild, key); // 如果key大于根节点就往右子树插入
    }else{
        return 0;
    }    
}

// 通过序列构建二叉排序树
void CreateBST(BiTree &T, ElemType *nums, int numsSize){
    T = NULL; // 如果没有构建就给NULL值，不分配空间
    // 开始遍历序列
    int i = 0;
    while(i < numsSize){
        BST_Insert_recursive(T, nums[i++]);
    }
}
int main(){
    BiTree T = NULL;
    CreateBiTree(T);
    BiTreeNode * resuilt =  BST_Search(T,3);
    if(resuilt == NULL){
        printf("没有找到\n");
    }else{
        printf("%d", resuilt->data);
    }
    PreOrderTraverse(T);
    printf("\n");
    BST_Insert(T,3);
    PreOrderTraverse(T);
    printf("\n");
    int nums[] = {1,2,3,4,5,6,7,8,9,10};
    CreateBST(T, nums, sizeof(nums)/sizeof(nums[0]));
    PreOrderTraverse(T);
    return 0;
}
