//借助循环顺序队列实现层次遍历
#include "BiTree.cpp"

typedef BiTreeNode ElemType_Queue;

typedef struct SqQueue
{
    ElemType_Queue *base;
    int front;
    int rear;
    int maxsize;
    int tag;  // 当舍弃一个空间时无用，用来标记当头尾相同时是否为空
}SqQueue;

int InitQueue(SqQueue &Q, int maxsize);
int SqQueueEmpty(SqQueue Q);
int SqQueueFull(SqQueue Q);
int EnSqQueue(SqQueue &Q, ElemType_Queue e);
int DeSqQueue(SqQueue &Q, ElemType_Queue &e);
int SqQueueLength(SqQueue Q);
int Display(SqQueue Q);


// ？？如何遍历队列中的元素？？
int Display(SqQueue Q){
    if(SqQueueEmpty(Q)){
        return 0;
    }
    // i只限定次数，下标由index另求
    int index = Q.front;
    int times = SqQueueLength(Q);
    for(int i = 0; i < times; i++, index++){
        index = index % Q.maxsize;
        printf("%d ",Q.base[index]);
    }
    printf("\n");
    return 1;
}


int SqQueueLength(SqQueue Q){
    // 如果使用tag就不必作此if判断
    if(SqQueueFull(Q)){
        return Q.maxsize;
    }
    return ((Q.rear + Q.maxsize - Q.front) % Q.maxsize); 
}


int InitQueue(SqQueue &Q, int maxsize){
    Q.base = (ElemType_Queue *)malloc(maxsize * sizeof(ElemType_Queue));
    if(!Q.base){
        printf("malloc error!\n");
        return 0;
    }
    Q.front = 0;
    Q.rear = Q.front;
    Q.maxsize = maxsize;
    return 1;
}

int SqQueueEmpty(SqQueue Q){
    if(Q.front == Q.rear){ // 队空
        printf("Queue Empty\n");
        return 1;
    }
    return 0;
}

int SqQueueFull(SqQueue Q){
    if((Q.rear + 1) % Q.maxsize == Q.front){ // 队满
        printf("Queue Full\n");
        return 1;
    }
    return 0;
}

int EnSqQueue(SqQueue &Q, ElemType_Queue e){
    if(SqQueueFull(Q)){
        return 0;
    }
    Q.base[Q.rear] = e;
    Q.rear = (Q.rear + 1) % Q.maxsize;
    return 1;
}

int DeSqQueue(SqQueue &Q, ElemType_Queue &e){
    if(SqQueueEmpty(Q)){
        return 0;
    }
    e = Q.base[Q.front];
    Q.front = (Q.front + 1) % Q.maxsize;
    return 1;
}


int visit(BiTreeNode * p){
    printf("%d ", p->data);
    return 1;
}

// 每次while循环只访问一个，但是将这个结点的所有子结点都放到队列中，能保证遍历完某层的所有结点后，下一层结点刚好全部进入队列（且此时队列中只有一层结点），
void LevelOrder(BiTree T){
    SqQueue Q;
    InitQueue(Q, 65); // 初始化辅助队列, 队列长度应该使用2^(h-1)
    BiTreeNode *p = T;
    EnSqQueue(Q, *p);
    // 循环输当前队列（层次）中所有结点
    while(!SqQueueEmpty(Q)){
        DeSqQueue(Q, *p);
        visit(p);
        if(p->lchild){
            EnSqQueue(Q, *p->lchild);
        }
        if(p->rchild){
            EnSqQueue(Q, *p->rchild);
        }
        // ... 如果结点有其他指针域，则需要遍历完所有指针域
    }

}

int main(){
    BiTree T;
    CreateBiTree(T);
    LevelOrder(T);
    return 0;
}