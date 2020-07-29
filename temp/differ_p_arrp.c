/*测试指针和数组指针的不同*/
#include <stdio.h>
typedef struct AA
{
    const char *name;
    int *area;
} AA;
typedef struct BB
{
    const char *name;
    int area[];
} BB;
int main(int argc, char const *argv[])
{

    int arr[3] = {1, 2, 3};
    int *p = &arr[0];
    int *pp = arr; /*其实这并不是整个数组的指针*/
    printf("arr: 0x%p, p: 0x%p, pp: 0x%p\n", arr, p, pp);
    printf("arr==p?%d\n", arr == p);
    printf("arr==pp?%d\n", arr == p);
    AA a = {"eee", arr};
    AA b = {"eewaf",
            {1, 3, 4}};
    AA c = {"eee", p};
    return 0;
}
