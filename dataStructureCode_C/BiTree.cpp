#include <stdio.h>
#include <stdlib.h>
typedef int ElemType;
typedef struct BiTreeNode
{
    ElemType data;
    BiTreeNode *lchild, *rchild;
}BiTreeNode, *BiTree;

int CreateBiTree(BiTree &T);
int PreOrderTraverse(BiTree T); 
int InOrderTraverse(BiTree T); 
int PostOrderTraverse(BiTree T); 
// int LevelOrderTraverse(BiTree T);
int DestroyBiTree(BiTree &T);

// 先序遍历递归方式构建树 data --> lchild --> rchild
int CreateBiTree(BiTree &T){
    ElemType input;
    scanf("%d",&input);
    if(input == -1){
        T = NULL;
        return 0;
    }
    T = (BiTreeNode *)malloc(sizeof(BiTreeNode));
    if(!T){
        printf("malloc error\n");
        return 0;
    }
    T->data = input;
    CreateBiTree(T->lchild);
    CreateBiTree(T->rchild);
    return 1;
}
int isBiTreeEmpty(BiTree T){
    if(T == NULL){
        printf("BiTree is empty\n");
        return 1;
    }
    return 0;
}
// //二叉树的递归模板
// int function(T){
//     if(T){
//         //处理T->data;
//         function(T->lchild);
//         function(T->rchild);
//         return 1;
//     }
//     return 0; // 找到空指针
// }

// 先序遍历data --> lchild --> rchild
int PreOrderTraverse(BiTree T){
    if(T){
        printf("%d ",T->data);
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
        return 1;
    }
    return 0;
}
// 中序遍历T->lchild -- T->data -- T->rchild
int InOrderTraverse(BiTree T){
     if(T){
        PreOrderTraverse(T->lchild);
        printf("%d ",T->data);
        PreOrderTraverse(T->rchild);
        return 1;
    }
    return 0;
}
// 后序遍历T->lchild -- T->rchild -- T->data
int PostOrderTraverse(BiTree T){
    if(T){
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
        printf("%d ",T->data);
        return 1;
    }
    return 0;
}
// 遍历销毁树T->lchild -- T->rchild
int DestroyBiTree(BiTree &T){
    if(T){
        DestroyBiTree(T->lchild);
        DestroyBiTree(T->rchild);
        free(T);// 释放当前结点的空间，清除data的数值
        T = NULL;// 虽然T已经没有空间了，但是不把T赋值为NULL的话，T会有一个不可预料值
    }
    return 0;
}

int main(){
    BiTree MT;
    CreateBiTree(MT);
    if(!isBiTreeEmpty(MT)){
        PreOrderTraverse(MT);
        printf("\n");
        InOrderTraverse(MT);
        printf("\n");
        PostOrderTraverse(MT);
        printf("\n");
    }
    DestroyBiTree(MT);
    PreOrderTraverse(MT);
    return 0;
}