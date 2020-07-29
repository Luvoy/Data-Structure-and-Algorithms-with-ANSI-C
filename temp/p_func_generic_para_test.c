/*测试万能的函数指针可不可行*/
#include <stdio.h>
#include <stdlib.h>
int sum(int a, int b)
{
    return a + b;
}
void print(int a, int b)
{
    printf("%d %d\n", a, b);
}
int main(int argc, char const *argv[])
{
    /* typedef void *(*p_func_type)(void *); */
    /* p_func_type my_func = sum; */
    /*  my_func(1, 2); */
    typedef void *(*p_func_type)(void *, void *);
    p_func_type my_func = sum;
    printf("%d\n", my_func(1, 2));
    typedef void *(*p_func_type_2)(void *, ...);
    p_func_type_2 my_func_2 = sum;
    printf("%d\n", my_func_2(1, 2));
    /*至少个数对应*/
    p_func_type_2 my_func_3 = print;
    my_func_3(1, 2);
    printf("%d\n", (*my_func_2)(1, 2));
    return 0;
}
