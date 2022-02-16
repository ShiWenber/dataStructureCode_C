#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INFINITY INT_MAX // 无穷大，根据权的数据类型来设定，设定为该类型的最大值
typedef int VertexType; // 顶点存的数据的类型
typedef int WeightType; // 权重
// typedef int InfoType; // 弧相关信息的类型
typedef enum {Digraph, Undigraph, Dinetwork, Undinetwork} GraphKind; // 这是一个枚举类型，用来选择图的类型{有向图，有向网，无向网，无向图}
//邻接矩阵的中单元的定义（弧的定义）
//弧单元的定义
typedef struct ArcCell{
    WeightType weight; // 权重
    int adjecent; // 【英语知识：邻接 n. adjacency  v. adjecent】 当在无权图中使用时，一般用int，起到代替bool的效果（取值为1，0表示是，否连接），当作bool用可以取名adjecent或者isAdjecent(使用形容词或者is形容词),在有权图中，可以改变量名为weight表示权重，一般表示有无连接或权重多少
    // InfoType * info; // 指向该弧其他信息的指针，存的内容相当于序偶中的谓词信息<离散数学>，也可以给ArcCell添加其他属性
}ArcCell; // 图的邻接矩阵存储结构
// MGraph表示矩阵表示的图
typedef struct {
    VertexType *vertexs; // 顶点表
    ArcCell *arcs; // 邻接矩阵（边表）,定义指针的同时确定了分配的空间，一旦声明，自带空间，不需要再malloc，相当于ArcCell arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM]
    // 图的两个要素：顶点表和边表
    int vertexNum,arcNum; // 图的当前顶点数和弧数(边数乘2)
    GraphKind kind; // 图的种类标志
}SqGraph;
// 初始化图
int CreateSqGraph(SqGraph &G, int vertexNum);
// 构造无向网
int CreateDigraph(SqGraph &G);
int CreateUndigraph(SqGraph &G);
int CreateDinetwork(SqGraph &G);
int CreateUndinetwork(SqGraph &G);
int LocateInVertexs(SqGraph G, VertexType v);
// 压缩矩阵（对称）
int CompressSyMatrix(ArcCell *Matrix);
int LocateIn(int i, int j);
int BubbleSort(VertexType *vertexs);

int CreateSqGraph(SqGraph &G, int vertexNum){
    G.arcNum = 0;
    G.vertexNum = vertexNum;
    G.vertexs = (VertexType *)malloc(sizeof(VertexType) * (G.vertexNum + 1)); // 多出来的空间用来存哨兵
    for(int i = 0; i < G.vertexNum; i++){
        scanf("%d",&G.vertexs[i]);
    }
    // BubbleSort(G.vertexs); // 对顶点表排序
    G.arcs = (ArcCell *)malloc(sizeof(ArcCell) * G.vertexNum * G.vertexNum);
    for(int i = 0; i < G.vertexNum * G.vertexNum; i++){
        G.arcs[i] = {INFINITY, 0};
    }
    int kind;
    scanf("%d", &kind);
    if(0 <= kind && kind <= 3){
        G.kind = (GraphKind)kind;
        switch (G.kind)
        {
            case 0:
                return CreateDigraph(G);
            case 1:
                return CreateUndigraph(G);
            case 2:
                return CreateDinetwork(G);
            case 3:
                return CreateUndinetwork(G);
            default:
                return 0;
        }
    }else{
        printf("输入错误！\n");
        return 0;
    }
}

int CreateUndinetwork(SqGraph &G){
    CompressSyMatrix(G);
    // 压缩完毕，只有下三角的内容
    WeightType inputWeight;
    VertexType v1, v2;
    int i, j;
    while(scanf("%d %d %d", v1, v2, inputWeight) != EOF){// end of file
        i = LocateInVertexs(G, v1);
        j = LocateInVertexs(G, v2);
        G.arcs[LocateInCompressedSyMatrix(i,j)].weight = inputWeight;
        // G.arcs[LocateInCompressedSyMatrix(j,i)].weight = inputWeight;
    }
    return 1;
}

int CreateUndigraph(SqGraph &G){
    CompressSyMatrix(G);
    int inputAdjacent;
    VertexType v1, v2;
    int i, j;
    while(scanf("%d %d %d", v1, v2, inputAdjacent) != EOF){
        i = LocateInVertexs(G, v1);
        j = LocateInVertexs(G, v2);
        G.arcs[LocateInCompressedSyMatrix(i,j)].adjecent = inputAdjacent;
    }
    return 1;
}

int CreateDigraph(SqGraph &G){
    //CompressMMatrix(G); // 稀疏矩阵压缩
    int inputAdjacent;
    VertexType v1, v2;
    int i, j;
    while(scanf("%d %d %d", v1, v2, inputAdjacent) != EOF){
        i = LocateInVertexs(G, v1);
        j = LocateInVertexs(G, v2);
        G.arcs[LocateInCompressedSyMatrix(i,j)].adjecent = inputAdjacent;
    }
    return 1;
}


int CreateDinetwork(SqGraph &G){
    //CompressMMatrix(G); // 稀疏矩阵压缩
    WeightType inputWeight;
    VertexType v1, v2;
    int i, j;
    while(scanf("%d %d %d", v1, v2, inputWeight) != EOF){// end of file
        i = LocateInVertexs(G, v1);
        j = LocateInVertexs(G, v2);
        G.arcs[LocateInCompressedSyMatrix(i,j)].weight = inputWeight;
        // G.arcs[LocateInCompressedSyMatrix(j,i)].weight = inputWeight;
    }
    return 1;
}
// 哨兵位置静态查找法
int LocateInVertexs(SqGraph G, VertexType v){
    int result = 0;
    G.vertexs[G.vertexNum] = v;
    for(int i = 0; ; i++){
        if(G.vertexs[i] == v){
            result = i;
            break;
        }
    }
    if(result == G.vertexNum){
        return 0;
    }else{
        return result;
    }
}
// 压缩对称阵
int CompressSyMatrix(SqGraph &G){
    ArcCell *Matrix = G.arcs;
    int vertexNum = G.vertexNum;
    ArcCell *CompressedMatrix = (ArcCell *)malloc(sizeof(ArcCell) * vertexNum * (vertexNum + 1) / 2);
    // 输入合法性判断
    if(Matrix == NULL){
        return 0;
    }
    int k = 0;
    for(int i = 0; i < vertexNum; i++){
        for(int j = 0; j < i + 1; i++){
            CompressedMatrix[k++] = Matrix[LocateInCompressedSyMatrix(i,j)];
        }
    }
    G.arcs = CompressedMatrix;
    return 1;
}

int LocateInCompressedSyMatrix(int i, int j){
    if(i >= j){
        //在下三角
        return (i * (i + 1) / 2 + j);
    }else{
        // i < j，在上三角
        return (j * (j + 1) / 2 + i);
    }
}















// int CreateSqGraph(SqGraph &G, int vertexNum){
//     G.vertexNum = vertexNum;
//     G.arcNum = 0; // 先初始化为0，因为没有存东西
//     G.vertexs = (VertexType *)malloc(sizeof(VertexType) * vertexNum + 1); // 多的一个空间是哨兵位置
//     for(int i = 0; i < G.vertexNum; i++ ){
//         scanf("%d", &G.vertexs[i]);
//     }
//     G.arcs = (ArcCell *)malloc(sizeof(ArcCell) * vertexNum * vertexNum);
//     int kind;
//     printf("Graph's kind: \n");
//     printf("1. digraph\n");
//     printf("2. undigraph\n");
//     printf("3. dinetwork\n");
//     printf("4. undinetwork\n");
//     printf("Please input the kind of graph: ");
//     scanf("%d",&kind);
//     if( 0 <= kind || kind <= 3 ){
//         G.kind = (GraphKind)kind;
//         // 图的所有通用属性完成处理，接下来按照图的种类不同进行不同处理
//         switch (G.kind)
//         {
//             case Digraph:
//                 return CreateDigraph(G);
//             case Undigraph:
//                 return CreateUndigraph(G);
//             case Dinetwork:
//                 return CreateDinetwork(G);
//             case Undinetwork:
//                 return CreateUndinetwork(G);
//             default:
//                 return 0;
//         }
//     }else{
//         printf("Error: The kind of graph is not correct!\n");
//         return 0;
//     }
// }

// // 构建无向网
// int CreateUndinetwork(SqGraph &G, int vertexNum, int arcNum){
//     // 先初始化再根据输入来存值
//     for(int i = 0; i < G.vertexNum * G.vertexNum; i++){
//         G.arcs[i].weight = INFINITY; // 初始赋值为无穷大
//     }
//     // 或者用G.arcs[i][j] = {INFINITY, ...}来一次性给所有ArcCell属性赋值
//     VertexType v1, v2;// 对有向图来说，v1是起点，v2是终点，对无向图来说，v1和v2是一样的
//     WeightType weight;
//     int i, j;
//     while(scanf("%d %d %d",&v1, &v2, &weight) != EOF){
//         i = LocateInVertexs(G, v1);
//         j = LocateInVertexs(G, v2);
//     }
// }
// // 输入结点值，返回结点在vertexs表中的位置（从0开始）
// int LocateInVertexs(SqGraph G, VertexType v){
//     G.vertexs[G.vertexNum] = v; // 哨兵位置
//     for(int i = 0; i < G.vertexNum; i++){
//         if(G.vertexs[i] == v){
//             return i;
//         }
//     }
// }





























