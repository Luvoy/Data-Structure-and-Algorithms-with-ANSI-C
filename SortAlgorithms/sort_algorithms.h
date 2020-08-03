#ifndef _SORT_ALGORITHMS_H_
#define _SORT_ALGORITHMS_H_
/*
使用方法:
arr中的元素类型必须一致, 需要实现int (*compare)(const void *, const void *)函数
升序降序也通过compare函数实现
*/
#include <stdint.h>
#ifndef SORT_INDEX_TYPE
#define SORT_INDEX_TYPE
typedef uint32_t sort_index_type;
#endif

#ifndef ELEM_SIZE_TYPE
#define ELEM_SIZE_TYPE
typedef uint32_t elem_size_type;
#endif

extern void swap(void *a, void *b, elem_size_type);
extern int compare(const void *a, const void *b);

extern void bubble_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *));

#endif