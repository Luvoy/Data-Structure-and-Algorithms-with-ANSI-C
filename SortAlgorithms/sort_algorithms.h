#ifndef _SORT_ALGORITHMS_H_
#define _SORT_ALGORITHMS_H_
/*
使用方法:
1. 修改元素类型, 必须改成基础类型或指针类型. 直接用struct是不对的. 不修改默认int
#ifdef ELEMENT_TYPE
#define elem_type [you want]
#endif

2. 
实现int (*compare)(const void *, const void *)函数
已给出的compare函数, 默认使用memcmp(a,b, sizeof(elem_type)), 升序;

升序降序也通过compare函数实现
*/
#include <stdint.h>
#ifndef SORT_INDEX_TYPE
#define SORT_INDEX_TYPE
typedef uint32_t sort_index_type;
#endif

#ifndef ELEMENT_TYPE
#define ELEMENT_TYPE
#define elem_type int
#endif

extern void swap(void *a, void *b);
extern int compare(const void *a, const void *b);

extern void bubble_sort(void *arr, sort_index_type size, int (*compare)(const void *, const void *));

#endif