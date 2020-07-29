/*测试在函数中开辟堆内存, 如果调用者不释放*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
unsigned char *func(unsigned int n)
{
    unsigned char *p = (unsigned char *)calloc(n, sizeof(unsigned char));
    assert(p);
    return p;
}

int main(int argc, char const *argv[])
{
    printf("0x%p\n", func((unsigned int)(4 * 1024 * 1024 * 1024 + 1)));
    printf("haha\n");
    return 0;
}
