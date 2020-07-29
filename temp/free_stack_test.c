/*测试释放栈上的内存*/
#include <stdio.h>
#include <stdlib.h>
void my_free(int *p)
{
    free(p);
}
int main(int argc, char const *argv[])
{
    int a = 0;
    int *p = &a;
    int *pp = (int *)malloc(3 * sizeof(int));
    printf("a: %d, &a: 0x%p\n", a, &a);
    printf("*pp: %d, pp: 0x%p\n", *pp, pp);
    *pp = 34;
    *(pp + 1) = 56;
    *(pp + 2) = 78;
    *p = 1;
    printf("a: %d, &a: 0x%p\n", a, &a);
    printf("*pp: %d, pp: 0x%p\n", *pp, pp);
    my_free(p);
    my_free(&a);
    free((void *)pp);                                                       /* 强制转换后, 只free了一个 */
    printf("after cast int* to void *, try to get pp[2]: %d\n", *(pp + 2)); /*仍能访问*/
    printf("after cast int* to void *, try to get pp[0]: %d\n", *(pp + 1));
    /* my_free(pp); */
    return 0;
}
