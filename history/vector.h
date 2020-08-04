#include "type.h"
#ifndef _VECTOR_H_
#define _VECTOR_H_
#ifndef MAX_ALLOCATE_SIZE
#define MAX_ALLOCATE_SIZE 8
#endif

typedef struct Vector
{ /*这是一种使用元素指针的、动态分配内存、内存连续的数组，
    元素类型可以在type.h中指定，一般为基础类型或指针类型，
    (也就是仅支持单一的数值类型, 而不支持结构体、多种基础类型组合的类型等)。*/
    size_type allocated_size;
    size_type used_size;
    element_type *items_p;
} Vector;

extern Vector *vector_new(size_type size);
extern Vector *vector_new_from_vector(const Vector *v);
extern Vector *vector_new_from_elem_array(element_type *p, size_type size);

extern void vector_print(const Vector *v, const char *format);

extern void vector_free(Vector *v);
#define vector_destroy(v) vector_free(v)

extern size_type vector_length(const Vector *v);
#define vector_len(v) vector_length(v)

extern element_type vector_elem_at(const Vector *v, size_type index, my_bool allow_negative_index);
#define vector_get(v, i, b) vector_elem_at((v), (i), (b))

extern void vector_index_assign(const Vector *v, size_type index, element_type elem);
#define vector_set(v, i, e) vector_index_assign((v), (i), (e))

extern size_type vector_index_of(const Vector *v, element_type elem);
#define vector_find(v, i) vector_index_of((v), (i))
#define vector_index(v, i) vector_index_of((v), (i))

extern void vector_append(Vector *v, element_type elem);
#define vector_push(v, e) vector_append((v), (e))

extern void vector_join(Vector *v_left, Vector *v_right);

extern element_type vector_pop(Vector *v);
#define vector_pop_last(v) vector_pop(v)

extern element_type vector_pop_i(Vector *v, size_t index);
#define vector_delete_i(v, i) vector_pop_i((v), (i))
#define vector_remove_i(v, i) vector_pop_i((v), (i))

extern void vector_insert(Vector *v, size_t i, element_type e);
#define vector_insert_i(v, i, e) vector_insert((v), (i), (e))
void vector_deep_copy(const Vector *v_src, Vector *v_des);
#define vector_copy(src, des) vector_deep_copy((src), (des))

extern Vector *vector_slice(Vector *v, size_type start, size_type end, size_type step);

extern void vector_reverse(Vector *v);

#endif