/* 在Vector中, deepcopy中调用deepcopy对象的函数
这个函数是用函数指针调用的, 参数在调用之前和传入之后发生了变化,
这是怎么回事?  现在来测试一下*/
#include <stdlib.h>
#include <stdio.h>

void sum(int *a, int *b, size_t n)
{
    int i = 0;
    for (i = 0; i < n; i++)
    {
        *(a + i) += *(b + i);
    }
}
int naive_sum(int a, int b)
{
    return a + b;
}
int p_sum(double *a, double *b)
{
    return *a + *b;
}
typedef void (*func)(void *a, void *b, size_t n);
typedef void *(*func2)(void *a, void *b, ...);
typedef void *(*func3)(void *a, void *b, ...);
int main(int argc, char const *argv[])
{
    func my_f = sum;
    int *a = calloc(3, sizeof(int));
    int *b = calloc(3, sizeof(int));
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    b[0] = 4;
    b[1] = 5;
    b[2] = 6;

    my_f(a, b, 3);
    int i;
    for (i = 0; i < 3; ++i)
    {

        printf("%d\n", a[i]);
    }

    func2 my_f2 = naive_sum;
    for (i = 0; i < 3; ++i)
    {
        int ret = 0;
        ret = my_f2(a[i], b[i]);
        printf("%d\n", ret);
    }
    func3 my_f3 = p_sum;

    for (i = 0; i < 3; ++i)
    {
        int ret = 0;
        ret = my_f3(a + i, b + i);
        printf("%d\n", ret);
    }
    return 0;
}
