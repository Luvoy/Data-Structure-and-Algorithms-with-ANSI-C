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
#ifndef _VECTOR_PTYPE_H_
#define _VECTOR_PTYPE_H_

#ifndef VECTOR_PTYPE_MAX_ALLOCATE_SIZE
#define VECTOR_PTYPE_MAX_ALLOCATE_SIZE 9
#endif

#ifndef VECTOR_PTYPE_NOT_FOUND_INDEX
#define VECTOR_PTYPE_NOT_FOUND_INDEX -1
#endif

#ifndef VECTOR_PTYPE_INDEX_TYPE
#define VECTOR_PTYPE_INDEX_TYPE
typedef int32_t vector_ptype_index_type;
#endif

typedef struct VectorPtype
{ /* 这是一种使用万能指针的、动态分配内存、内存连续的Vector；
    element类型是void*，意味着此Vector仅支持指针类型    */
  vector_ptype_index_type allocated_size;
  vector_ptype_index_type used_size;
  void **items_p;
} VectorPtype;
extern VectorPtype *vector_ptype_new(const vector_ptype_index_type size);

extern VectorPtype *vector_ptype_new_from_vector(const VectorPtype *v);
extern VectorPtype *vector_ptype_new_from_args(vector_ptype_index_type size, ...);
extern VectorPtype *vector_ptype_new_from_elements(vector_ptype_index_type size, void **p);

extern void vector_ptype_print(const VectorPtype *v, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *));

extern void vector_ptype_free(VectorPtype **v);

extern vector_ptype_index_type vector_ptype_length(const VectorPtype *v);
extern void *vector_ptype_element_at(const VectorPtype *v, vector_ptype_index_type index, my_bool allow_negative_index);
extern void vector_ptype_index_assign(const VectorPtype *v, vector_ptype_index_type index, void *elem);
extern vector_ptype_index_type vector_ptype_left_search(const VectorPtype *v, void *key, my_bool (*elem_equal)(const void *, const void *));
extern void vector_ptype_append(VectorPtype *v, void *elem);
extern void vector_ptype_join(VectorPtype *v_left, VectorPtype *v_right);
extern void *vector_ptype_pop(VectorPtype *v);
extern void *vector_ptype_pop_i(VectorPtype *v, vector_ptype_index_type index);
extern void vector_ptype_insert(VectorPtype *v, vector_ptype_index_type index, void *elem);
extern VectorPtype *vector_ptype_slice(VectorPtype *v, vector_ptype_index_type start, vector_ptype_index_type end, vector_ptype_index_type step);
extern void vector_ptype_reverse(VectorPtype *v);

#endif