#include "sort_algorithms.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
extern void swap(void *a, void *b)
{
    assert(a);
    assert(b);
    elem_type temp = *(elem_type *)(a);
    *(elem_type *)(a) = *(elem_type *)(b);
    *(elem_type *)(b) = temp;
}

extern int compare(const void *a, const void *b)
{
    return memcmp(a, b, sizeof(elem_type));
}

extern void bubble_sort(void *arr, sort_index_type arr_size, int (*compare)(const void *, const void *))
{
    assert(arr);
    elem_type *arr_cast = (elem_type *)arr;
    if (arr_size <= 1)
    {
        return;
    }
    sort_index_type i, j;
    for (i = 0; i < arr_size - 1; ++i)
    {
        for (j = 0; j < arr_size - i - 1; ++j)
        {
            printf("%d, ", compare((arr_cast + j), (arr_cast + j + 1)));
            if (compare((arr_cast + j), (arr_cast + j + 1)) > 0)
            { /*默认升序*/
                printf("haha");
                swap((arr_cast + j), (arr_cast + j + 1));
            }
        }
        printf("\n");
    }
}
