#include "sort_algorithms.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
extern void swap(void *a, void *b, elem_size_type elem_size)
{
    assert(a);
    assert(b);
    void *temp = malloc(elem_size);
    assert(temp);
    memcpy(temp, a, elem_size);
    memcpy(a, b, elem_size);
    memcpy(b, temp, elem_size);
    free(temp);
}

extern int compare(const void *a, const void *b)
{ /*example*/
    return 0;
}

extern void bubble_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *))
{
    assert(arr);
    uint8_t *arr_cast = (uint8_t *)(arr);
    if (arr_size <= 1)
    {
        return;
    }
    sort_index_type i, j;
    for (i = 0; i < arr_size - 1; ++i)
    {
        for (j = 0; j < arr_size - i - 1; ++j)
        {
            if (compare((arr_cast + j * elem_size), (arr_cast + (j + 1) * elem_size)) > 0)
            { /*默认升序*/
                swap((arr_cast + j * elem_size), (arr_cast + (j + 1) * elem_size), elem_size);
            }
        }
    }
}
