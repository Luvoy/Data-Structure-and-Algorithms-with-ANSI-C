/*结构体嵌套的访问*/
#include <stdio.h>
#include <stdlib.h>
typedef struct A
{
    int size;
} A;
typedef struct AA
{
    A *a;
} AA;
typedef struct AAA
{
    AA *aa;
} AAA;
int main(int argc, char const *argv[])
{
    A *ap = (A *)calloc(1, sizeof(A));
    AA *aap = (AA *)calloc(1, sizeof(AA));
    AAA *aaap = (AAA *)calloc(1, sizeof(AAA));
    aaap->aa = aap;
    aaap->aa->a = ap;
    /*  aap->a = ap; */
    aaap->aa->a->size = 1;
    printf("%d\n", aaap->aa->a->size);
    return 0;
}
