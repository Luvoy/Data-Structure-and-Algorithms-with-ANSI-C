#ifndef _SORT_ALGORITHMS_H_
#define _SORT_ALGORITHMS_H_
/*
使用方法:
arr中的元素类型必须一致, 需要实现int (*compare)(const void *, const void *)函数
升序降序也通过compare函数实现
默认升序时，compare(a,b)>0作为改变条件，类似于a-b>0的顺序
*/
#include <stdint.h>
#ifndef SORT_INDEX_TYPE
#define SORT_INDEX_TYPE
typedef int32_t sort_index_type; /* 负值是有用的！*/
#endif

#ifndef ELEM_SIZE_TYPE
#define ELEM_SIZE_TYPE
typedef uint32_t elem_size_type;
#endif

extern int compare(const void *a, const void *b);

extern void bubble_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *));
extern void bubble_sort_improve(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *));
extern void select_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *));
extern void insert_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *));
extern void shell_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *));
extern void quick_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *));
extern void heap_sort(void *arr, sort_index_type arr_size, elem_size_type elem_size, int (*compare)(const void *, const void *));

#endif