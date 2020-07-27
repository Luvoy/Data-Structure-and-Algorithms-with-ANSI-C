#include "../object.h"
#include "../my_bool.h"
#ifndef _VECTOR_OBJECT_H_
#define _VECTOR_OBJECT_H_

#ifndef MAX_ALLOCATE_SIZE
#define MAX_ALLOCATE_SIZE 80
#endif

typedef struct Vector
{
    /**
     * 这是一种使用万能指针的、动态分配内存、内存连续的Vector；
     * object类型可以调用时指定，但仅支持同一种的变量类型；
     * 既支持基础类型或指针类型数值类型, 又支持结构体、多种基础类型组合的类型等，但传入的都是void*；
     * 需要实现单个对象的打印, 分配空间, free等操作；
     * 需要把上述这些操作函数的函数指针复制到一个ObjectFuncs结构体里，Vector需要这个结构体；
     * 具体的object和Vector之间的很多操作通过强制转换实现；
     */
    size_type allocated_size;
    size_type used_size;
    void **items_p;
    ObjectFuncs *obj_funcs;

} Vector;

Vector *vector_new(const size_type size, ObjectFuncs *o);
void **vector_calloc_data(size_type size);
Vector *vector_new_from_vector(const Vector *v);
Vector *vector_new_from_args(ObjectFuncs *o, int size, ...);
Vector *vector_new_from_objects(void **p, size_type size, ObjectFuncs *o);
void vector_print(const Vector *v, const char *format);
void vector_free(Vector *v);
void vector_free_data(Vector *v);
size_type vector_length(const Vector *v);
void *vector_object_at(const Vector *v, size_type index, my_bool allow_negative_index);
void vector_index_assign(const Vector *v, size_type index, void *object);
size_type vector_left_search(const Vector *v, void *object);
void vector_append(Vector *v, void *object);
void vector_join(Vector *v_left, Vector *v_right);
void *vector_pop(Vector *v);
void *vector_pop_i(Vector *v, size_type index);
void vector_insert(Vector *v, size_t index, void *object);
void vector_deep_copy(const Vector *v_src, Vector *v_des);
Vector *vector_slice(Vector *v, size_type start, size_type end, size_type step, ObjectFuncs *o);
void vector_reverse(Vector *v, size_type size);

#endif