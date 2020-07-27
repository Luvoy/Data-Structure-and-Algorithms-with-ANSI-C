#include <stdio.h>
#include <stdlib.h>
#include "str_func_test.h"
void str_func_test()
{
    int a = 0;
    int b = 32414;
    int c = 128;
    int d = -127;
    int e = 0xA341;
    int f = -128;
    char *a_ = my_itoa(a, 10);
    printf("%s\n", a_);
    char *b_ = my_itoa(b, 10);
    printf("%s\n", b_);
    char *c_ = my_itoa(c, 2);
    printf("%s\n", c_);
    char *d_ = my_itoa(d, 2);
    printf("%s\n", d_);
    char *e_ = my_itoa(e, 16);
    printf("%s\n", e_);
    char *f_ = my_itoa(f, 2);
    printf("%s\n", f_);
    free(a_);
    free(b_);
    free(c_);
    free(d_);
    free(e_);
    free(f_);
}