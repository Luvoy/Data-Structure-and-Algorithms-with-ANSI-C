#include <stdio.h>

int main(int argc, char const *argv[])
{
    typedef int $myint;
    typedef int _myint;
    int $a;
    /* typedef int 24int; */
    $myint a = 1;
    _myint b = 2;
    $a = 3;
    printf("%d %d %d\n", a, b, $a);
    return 0;
}
