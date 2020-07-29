/*指针强制转换, 步长变化*/
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
    int a[] = {2, 43534, 45642, 42356, 74575};
    void *p = &a[0];
    void *pp = (int *)p;
    printf("p: %p, *p: %d\n", p);
    printf("pp: %p, *pp: %d\n", pp);

    printf("pp + 1 \npp: %p, *pp: %d\n", pp + 1);
    printf("pp + sizeof(int) \npp: %p, *pp: %d\n", pp + sizeof(int));
    char *ppp = (char *)p;
    printf("ppp: %p, *ppp: %d\n", ppp);

    printf("ppp + 1 \nppp: %p, *ppp: %d\n", ppp + 1);
    printf("ppp + sizeof(int) \nppp: %p, *ppp: %d\n", ppp + sizeof(int));
    return 0;
}
