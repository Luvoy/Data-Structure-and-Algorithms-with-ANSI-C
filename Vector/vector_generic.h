#include "../my_bool.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#ifndef _VECTOR_GENERIC_H_
#define _VECTOR_GENERIC_H_

#ifndef VECTOR_GENERIC_MAX_ALLOCATE_SIZE
#define VECTOR_GENERIC_MAX_ALLOCATE_SIZE 9
#endif

#ifndef VECTOR_GENERIC_NOT_FOUND_INDEX
#define VECTOR_GENERIC_NOT_FOUND_INDEX -1
#endif

#ifndef VECTOR_GENERIC_INDEX_TYPE
#define VECTOR_GENERIC_INDEX_TYPE
typedef int64_t vector_generic_index_type;
#endif

#ifndef ELEM_SIZE_TYPE
#define ELEM_SIZE_TYPE
typedef int64_t elem_size_type;
#endif

typedef struct VectorGeneric
{                                           /* 这是一种泛型的、动态分配内存、内存连续的Vector；
    仅支持同一种类型，需要提前指定元素的字节大小（通常用sizeof即可） */
  vector_generic_index_type allocated_size; /*分配的元素个数, 不是内存字节数*/
  vector_generic_index_type used_size;      /*使用的元素个数*/
  void *items_p;
  elem_size_type elem_size;
} VectorGeneric;

extern VectorGeneric *vector_generic_new(const vector_generic_index_type size, elem_size_type elem_size);

extern VectorGeneric *vector_generic_new_from_vector(const VectorGeneric *v);
extern VectorGeneric *vector_generic_new_from_args(vector_generic_index_type size, elem_size_type elem_size, ...);
extern VectorGeneric *vector_generic_new_from_elements(vector_generic_index_type size, elem_size_type elem_size, void *p);

extern void vector_generic_print(const VectorGeneric *v, FILE *f, const char *format, void (*elem_print)(FILE *, const void *));

extern void vector_generic_free(VectorGeneric **v);

extern vector_generic_index_type vector_generic_length(const VectorGeneric *v);
extern void *vector_generic_element_at(const VectorGeneric *v, vector_generic_index_type index, my_bool allow_negative_index);
extern void vector_generic_index_assign(const VectorGeneric *v, vector_generic_index_type index, void *elem);
extern vector_generic_index_type vector_generic_left_search(const VectorGeneric *v, void *key, my_bool (*elem_equal)(const void *, const void *));
extern void vector_generic_append(VectorGeneric *v, void *elem);
extern void vector_generic_join(VectorGeneric *v_left, VectorGeneric *v_right);
extern void *vector_generic_pop(VectorGeneric *v);
extern void *vector_generic_pop_i(VectorGeneric *v, vector_generic_index_type index);
extern void vector_generic_insert(VectorGeneric *v, vector_generic_index_type index, void *elem);
extern VectorGeneric *vector_generic_slice(VectorGeneric *v, vector_generic_index_type start, vector_generic_index_type end, vector_generic_index_type step);
extern void vector_generic_reverse(VectorGeneric *v);
extern void vector_generic_remove(VectorGeneric *v, void *elem, int (*compare)(const void *, const void *), vector_generic_index_type times);

#endif