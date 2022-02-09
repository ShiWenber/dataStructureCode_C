#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h> // 只在需要变长参数表的时候使用，提供宏va_start，va_arg，va_end（三个操作va_list的函数）和va_list(一个类型，能够提供指向变长参数表的指针)
/*
    <stdarg.h>中的定义：
    va_list类型：是一个指针，指向变长参数表的首地址。  
    type va_arg( va_list argptr, type );  
    void va_end( va_list argptr );  
    void va_start( va_list argptr, last_parm ); // last_parm是...前的最后一个参数
    功能： 宏va_arg()用于给函数传递可变长度的参数列表。 
    首先，必须调用va_start() 传递有效的参数列表va_list和函数强制的第一个参数。第一个参数代表将要传递的参数的个数。 
    (注意：这里的第一个参数指的是在va_start()参数中指定的va_list参数表的起始参数，而不是va_start所在外部函数的第一个参数，即last_parm 是最后一个传递给函数的已知的固定参数，即省略号之前的参数。)
    其次，调用va_arg()传递参数列表va_list 和将被返回的参数的类型。va_arg()的返回值是当前的参数。 
    再次，对所有的参数重复调用va_arg() 
    最后，调用va_end()传递va_list对完成后的清除是必须的。 
    For example: 

        int sum( int, ... );

        int main( void ) {

        

        int answer = sum( 4, 4, 3, 2, 1 );

        printf( "The answer is %d\n", answer );

        

        return( 0 );

        }

        

        int sum( int num, ... ) {

        int answer = 0;

        va_list argptr;

        

        va_start( argptr, num );

        for( ; num > 0; num-- )

            answer += va_arg( argptr, int );

        

        va_end( argptr );

        return( answer );

        }

*/

#define MAX_ARRAY_DIM 8 // 数组维数最大为8
typedef int ElemType;
typedef struct
{
    ElemType *base; // 空间基址,所有下标为0时指向的空间
    int dim; // 数组维数,标志一个数据元素所需要的下标个数
    int *bounds; // 数组维界，每个维度的范围，bounds[i]表示第i+1维的范围即单元数
    int *constants; // 数组映像函数常量基址，constants[i]表示第i+1维的单元长度，根据映像函数的公式，constants[i] = bounds[i+1] * constants[i+1] , constants[dim-1] = 1（表示一个ElemType）bounds和constants[dim-1]是容易知道的，因此求constans需要从dim-1开始求，下标从dim-1到0
    int elemtotal; // 数组可存储的元素总数
}Array;

//初始化后没有给空间中赋初值
int InitArray(Array &A, int dim, ...){
    if(dim < 1 || dim > MAX_ARRAY_DIM)
        return 0;
    A.dim = dim;
    A.bounds = (int *)malloc(dim * sizeof(int)); // dim个int类型的空间，用来存储每个维度的范围
    if(!A.bounds){
        return 0;
    }
    //////////////////变长参数表的一般操作
    int elemtotal = 1; // 存储数组元素总个数，方便分配空间
    va_list ap; // 变长参数表ap
    va_start(ap, dim); // 初始化变长参数表ap , dim表示已知的最后一个参数
    for(int i=0; i<dim; i++){
        A.bounds[i] = va_arg(ap, int); // int表示现在从参数列表取的值类型为int，va_arg会返回int类型的值
        if(A.bounds[i] <= 0){ // 合法性判断
            return 0;
        }
        elemtotal *= A.bounds[i];
    }
    va_end(ap);
    ////////////////////对参数列表的操作结束
    A.base = (ElemType *)malloc(elemtotal * sizeof(ElemType)); // 分配空间
    A.elemtotal = elemtotal;
    if(!A.base){
        printf("malloc error\n");
        return 0;
    }
    A.constants = (int *)malloc(dim * sizeof(int)); // 分配空间
    if(!A.constants){
        printf("malloc error\n");
        return 0;
    }
    A.constants[dim-1] = 1; // 初始化最后一个常量
    //变量方向的讨论见Array结构的定义处
    for(int i = dim-2; i >= 0; i--){
        A.constants[i] = A.bounds[i+1] * A.constants[i+1];
    }
    return 1;
}

//使A成为Init之前的状态
int DestroyArray(Array & A){
    if(A.base){
        free(A.base);
        A.base =NULL;
    }
    if(A.bounds){
        free(A.bounds);
        A.bounds = NULL;
    }
    if(A.constants){
        free(A.constants);
        A.constants = NULL;
    }
    A.dim = 0;
    A.elemtotal = 0;
    return 1;
}
//求出相对地址reAddress后可以用来访问的元素地址:A[reAddress],相当于通过一组下标定位A[d1][d2][d3]...[dn]
int Locate(Array A , va_list ap, int & reAddress){
    //若ap中指示的各下标值合法，则求出该元素在A中的相对地址reAddress
    reAddress = 0;
    for(int i = 0; i < A.dim; i++){
        int index = va_arg(ap,int);
        if(index < 0 || index >= A.bounds[i]){
            printf("index out of range\n");
            return 0;
        }
        reAddress += index * A.constants[i]; // 遍历单位数乘以该维度的常量（即单元长度）
    }
    return 1;
}

int Value(Array A, ElemType & e, ...){
    va_list ap;
    va_start(ap, e);
    int reAddress;
    int resuilt;
    if(Locate(A, ap, reAddress)){
        e = *(A.base+reAddress); // e = A.base[reAddress];
        va_end(ap);
        return 1;
    }else{
        va_end(ap);
        return 0;
    }
}
//赋值
int Assign(Array & A, ElemType e, ...){
    va_list ap;
    va_start(ap, e);
    int reAddress;
    if(Locate(A, ap, reAddress)){
        *(A.base+reAddress) = e; // A.base[reAddress] = e;
        va_end(ap);
        return 1;
    }else{
        va_end(ap);
        return 0;
    }
}
//展现数组
int DisplayArray(Array A){
    if(A.dim > 2){
        printf("数组维度超过二维，无法展示\n");
        return 0;
    }else if(A.dim == 0){
        printf("数组为空\n");
        return 0;
    }
    for(int i = 0; i < A.elemtotal; i++){
        printf("%d\t", *(A.base+i)); // A.base[i]
        if(i % A.bounds[A.dim - 1] == 3){
            printf("\n");
        }
    }
    return 1;
}

int AssignAll(Array & A, ElemType e){
    if(A.dim == 0){
        printf("数组为空\n");
        return 0;
    }
    for(int i = 0; i < A.elemtotal; i++){
        *(A.base+i) = e; // A.base[i] = e;
    }
    return 1;
}

int main(){
    Array A;
    InitArray(A, 2, 3, 4); // 初始化一个二维3*4的数组
    DisplayArray(A);
    Assign(A, 2, 1, 2); // 给A[1][2]赋值e
    ElemType e;
    Value(A, e, 1, 2); // 获取A[1][2]的值
    printf("%d\n", e);
    DisplayArray(A);
    AssignAll(A, 0); // 给A中所有元素赋值e
    DisplayArray(A);
    DestroyArray(A);
    return 0;
}