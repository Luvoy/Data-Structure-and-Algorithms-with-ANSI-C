#include "sort_algorithms.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

extern void bubble_sort_improve(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *))
{
    assert(arr);
    if (arr_size <= 1)
    {
        return;
    }
    sort_index_type left = 0;
    sort_index_type right = arr_size - 1;
    sort_index_type i;
    while (left < right)
    {
        for (i = left; i < right; ++i)
        {
            if (compare(arr + i * elem_size, arr + (i + 1) * elem_size) > 0)
            {
                swap(arr + i * elem_size, arr + (i + 1) * elem_size, elem_size);
            }
        }
        --right;

        for (i = right; i > left; --i)
        {
            if (compare(arr + (i - 1) * elem_size, arr + i * elem_size) > 0)
            {
                swap(arr + (i - 1) * elem_size, arr + i * elem_size, elem_size);
            }
        }
        ++left;
    }
}

extern void select_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *))
{
    assert(arr);
    if (arr_size <= 1)
    {
        return;
    }
    sort_index_type i, j;
    for (i = 0; i < arr_size - 1; ++i)
    {
        for (j = i + 1; j < arr_size; ++j)
        {
            if (compare(arr + i * elem_size, arr + j * elem_size) > 0)
            {
                swap(arr + i * elem_size, arr + j * elem_size, elem_size);
            }
        }
    }
}