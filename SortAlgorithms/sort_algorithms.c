#include "sort_algorithms.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
static void swap(void *a, void *b, elem_size_type elem_size)
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

extern void insert_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *))
{
    assert(arr);
    if (arr_size <= 1)
    {
        return;
    }
    sort_index_type i, j;
    for (i = 1; i < arr_size; ++i)
    {

        void *temp = malloc(elem_size);
        assert(temp);
        memcpy(temp, arr + i * elem_size, elem_size);
        for (j = i; j > 0 && compare(temp, arr + (j - 1) * elem_size) < 0; --j)
        {
            memcpy(arr + j * elem_size, arr + (j - 1) * elem_size, elem_size);
        }
        memcpy(arr + j * elem_size, temp, elem_size);
        free(temp);
    }
}

extern void shell_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *))
{
    assert(arr);
    if (arr_size <= 1)
    {
        return;
    }
    sort_index_type d, i, j;
    for (d = arr_size / 2; d >= 1; d /= 2)
    {
        for (i = d; i < arr_size; ++i)
        {
            void *temp = malloc(elem_size);
            memcpy(temp, arr + i * elem_size, elem_size);
            for (j = i - d; j >= 0 && compare(arr + j * elem_size, temp) > 0; j -= d)
            {
                memcpy(arr + (j + d) * elem_size, arr + j * elem_size, elem_size);
            }
            if (j != i - d)
            { /*说明确实移动了*/
                memcpy(arr + (j + d) * elem_size, temp, elem_size);
            }
            free(temp);
        }
    }
}

static sort_index_type _parse_subsequence(void *arr, sort_index_type left, sort_index_type right, elem_size_type elem_size, int (*compare)(const void *, const void *))
{
    void *base_copy = malloc(elem_size);
    memcpy(base_copy, arr + elem_size * left, elem_size);
    while (left < right)
    {
        while (left < right && compare(arr + elem_size * right, base_copy) > 0)
        {
            --right;
        }
        /* 提前赋值， 可以节省swap*/
        memcpy(arr + elem_size * left, arr + elem_size * right, elem_size);
        while (left < right && compare(arr + elem_size * left, base_copy) <= 0)
        {
            ++left;
        }
        memcpy(arr + elem_size * right, arr + elem_size * left, elem_size);
    }
    memcpy(arr + elem_size * left, base_copy, elem_size);
    free(base_copy);
    return left;
}

static void _quick_sort(void *arr, sort_index_type left, sort_index_type right, elem_size_type elem_size, int (*compare)(const void *, const void *))
{
    if (left >= right)
    {
        return;
    }
    sort_index_type base = _parse_subsequence(arr, left, right, elem_size, compare);
    _quick_sort(arr, left, base - 1, elem_size, compare);
    _quick_sort(arr, base + 1, right, elem_size, compare);
}

extern void quick_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *))
{
    assert(arr);
    if (arr_size <= 1)
    {
        return;
    }
    _quick_sort(arr, 0, arr_size - 1, elem_size, compare);
}

extern void heap_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *))
{
}