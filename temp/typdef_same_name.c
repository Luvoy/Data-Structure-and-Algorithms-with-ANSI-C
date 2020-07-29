#include <stdio.h>
void func(int a)
{
    printf("haha %d\n", a);
}
int main(int argc, char const *argv[])
{
    typedef void (*type_p_func)(int a);
    type_p_func p_func = func; /*p_func p_func;是不行的！ */
    int b = 1;
    p_func(b);
    return 0;
}
