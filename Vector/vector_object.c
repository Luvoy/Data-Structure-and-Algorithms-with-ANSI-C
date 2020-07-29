
#include "vector_object.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#define MY_DEBUG
Vector *vector_new(const size_type size, ObjectFuncs *o)
{
    Vector *v;
    assert(size > 0);
    v = (Vector *)calloc(1, sizeof(Vector));
    assert(v);
    v->allocated_size = ((size / MAX_ALLOCATE_SIZE) + 1) * MAX_ALLOCATE_SIZE;
    v->used_size = size;
    v->items_p = vector_calloc_data(v->allocated_size);
    v->obj_funcs = o;
    int i;
    for (i = 0; i < v->used_size; ++i)
    {
        *(v->items_p + i) = v->obj_funcs->p_func_object_alloc(v->obj_funcs->object_pointer_size);
    }
    return v;
}

Vector *vector_new_from_vector(const Vector *v)
{

    Vector *v_new = vector_new(v->used_size, v->obj_funcs);
#ifdef MY_DEBUG
    fprintf(stderr, "In [vector_new_from_vector] new vector: OK\n");
#endif
    vector_free_data(v_new);
    vector_deep_copy(v, v_new);
    return v_new;
}

Vector *vector_new_from_args(ObjectFuncs *o, int size, ...)
{
    assert(size > 0);
    Vector *v_new = vector_new(size, o);
#ifdef MY_DEBUG
    assert(v_new != NULL);
    fprintf(stderr, "In [vector_new_from_args] new vector: OK\n");
#endif
    va_list vl;
    va_start(vl, size);
    int i;
    for (i = 0; i < size; ++i)
    {
        *(v_new->items_p + i) = va_arg(vl, void *);
    }
    va_end(vl);
    return v_new;
}
Vector *vector_new_from_objects(void **p, size_type size, ObjectFuncs *o)
{
    Vector *v = vector_new(size, o);

#ifdef MY_DEBUG
    assert(v != NULL);
    fprintf(stderr, "In [vector_new_from_objects] new vector: OK\n");
#endif
    int i;
    for (i = 0; i < size; ++i)
    {
        *(v->items_p + i) = *(p + i);
    }
    return v;
}
void vector_print(const Vector *v, const char *format)
{
    if (v == NULL)
    {
        fprintf(stderr, "NULL vector!\n");
        return;
    }
    int i;
    if (strcmp(format, "SINGLE_LINE") == 0)
    {
        for (i = 0; i < v->used_size; ++i)
        {
            if (i == 0)
            {
                fprintf(stderr, "[");
            }
            v->obj_funcs->p_func_object_print(stderr, *((v->items_p) + i));
            if (i != v->used_size - 1)
            {

                fprintf(stderr, ", ");
            }
            else
            {
                fprintf(stderr, "]\n");
            }
        }
    }
    else if (strcmp(format, "MULTI_LINES") == 0)
    {
        for (i = 0; i < v->used_size; ++i)
        {
            v->obj_funcs->p_func_object_print(stderr, *((v->items_p) + i));
            fprintf(stderr, "\n");
        }
    }
    else
    {
        fprintf(stderr, "Undefined format string: %s\n", format);
    }
    return;
}

void vector_free(Vector *v)
{

    vector_free_data(v);
#ifdef MY_DEBUG
    fprintf(stderr, "In [vector_free] data: OK\n");
#endif
    /* free(v->obj_funcs);*/ /*这一句还是别加了, 因为new一个Vector的时候, Obj_funcs是直接指定的*/
    v->obj_funcs = NULL;
    free(v);
}

void vector_free_data(Vector *v)
{
    if (v->items_p)
    {
        if (v->used_size >= 0)
        {
            int i;
            for (i = 0; i < v->used_size; ++i)
            {
                v->obj_funcs->p_func_object_free(*(v->items_p + i));
                *(v->items_p + i) = NULL;
                /*隐患,可以会free一个栈内存*/
            }
        }
        free(v->items_p);
        v->items_p = NULL;
    }
}

size_type vector_length(const Vector *v)
{
    assert(v != NULL);
    return v->used_size;
}

void *vector_object_at(const Vector *v, size_type index, my_bool allow_negative_index)
{
    assert(v != NULL);
    if (!allow_negative_index)
    {
        if (index < 0 || index >= v->used_size)
        {
            fprintf(stderr, "Illegal index!\n");
            return NULL;
        }
        return *((v->items_p) + index);
    }
    else
    {
        if (index < -(v->used_size) || index >= v->used_size)
        {
            fprintf(stderr, "Illegal index!\n");
            return NULL;
        }
        size_type real_index = index > 0 ? index : v->used_size + index;
        return *((v->items_p) + real_index);
    }
}

void vector_index_assign(const Vector *v, size_type index, void *object)
{
    assert(v != NULL);
    if (index < 0 || index >= v->used_size)
    {
        fprintf(stderr, "Illegal index!\n");
        return;
    }
    *((v->items_p) + index) = object;
    return;
}

size_type vector_left_search(const Vector *v, void *object)
{
    assert(v != NULL);
    int i;
    for (i = 0; i < v->used_size; ++i)
    {
        if (v->obj_funcs->p_func_object_compare(object, *((v->items_p) + i)) == 0)
        {
            return i;
        }
    }
    return ERROR_SIZE;
}

void vector_append(Vector *v, void *object)
{
    assert(v != NULL);
    if (v->used_size < v->allocated_size)
    {
        *((v->items_p) + (v->used_size)) = object;
        v->used_size++;
    }
    else
    {
        void **temp = vector_calloc_data(v->allocated_size + MAX_ALLOCATE_SIZE);
        assert(temp);
        int i;
        for (i = 0; i < v->used_size; ++i)
        {
            *(temp + i) = *((v->items_p) + i);
        }
        *(temp + i) = object;
        vector_free_data(v);
        v->used_size++;
        v->items_p = temp;
    }
}

void vector_join(Vector *v_left, Vector *v_right)
{
    assert(v_left != NULL);
    assert(v_right != NULL);

    if (v_left->used_size + v_right->used_size <= v_left->allocated_size)
    {
        int i;
        for (i = 0; i < v_right->used_size; ++i)
        {
            *((v_left->items_p) + (v_left->used_size) + i) = *((v_right->items_p) + i);
        }
        v_left->used_size += v_right->used_size;

        /* vector_free(v_right); */
        /* v_right = NULL;  不起作用 */
    }
    else
    {
        void **temp = vector_calloc_data(MAX_ALLOCATE_SIZE * (((v_left->used_size) + (v_right->used_size)) / MAX_ALLOCATE_SIZE + 1));
        assert(temp);
        int i;
        for (i = 0; i < v_left->used_size; ++i)
        {
            *(temp + i) = *((v_left->items_p) + i);
        }
        for (i = 0; i < v_right->used_size; ++i)
        {
            *(temp + (v_left->used_size) + i) = *((v_right->items_p) + i);
        }
        v_left->used_size += v_right->used_size;

        vector_free_data(v_left);
        v_left->items_p = temp;

        /* vector_free(v_right); */
        /*v_right = NULL;  不起作用 */
    }
}

void *vector_pop(Vector *v)
{
    assert(v != NULL);
    if (v->used_size < 1)
    {
        fprintf(stderr, "No element to pop!\n");
        return NULL;
    }

    v->used_size--;
    return *((v->items_p) + v->used_size);
}

void *vector_pop_i(Vector *v, size_type index)
{
    assert(v != NULL);
    if (index < 0 || index > v->used_size - 1)
    {
        fprintf(stderr, "Illegal index!\n");
        return NULL;
    }
    int i;
    void *ret = *((v->items_p) + index);
    for (i = index; i < v->used_size - 1; ++i)
    {
        *((v->items_p) + i) = *((v->items_p) + i + 1);
    }
    *((v->items_p) + i) = 0;
    v->used_size--;
    return ret;
}

void vector_insert(Vector *v, size_t index, void *object)
{
    assert(v != NULL);
    if (index < 0 || index > v->used_size)
    {
        fprintf(stderr, "Illegal index!\n");
        return;
    }
    if (index == v->used_size)
    {
        vector_append(v, object);
        return;
    }
    else
    {
        int i;
        if (v->used_size == v->allocated_size)
        {
            void **temp = vector_calloc_data(v->allocated_size + MAX_ALLOCATE_SIZE);
            assert(temp);
            for (i = 0; i < index; ++i)
            {
                *(temp + i) = *((v->items_p) + i);
            }
            *(temp + i) = object;
            v->used_size++;
            for (++i; i < v->used_size; ++i)
            {
                *(temp + i) = *((v->items_p) + i - 1);
            }
            vector_free_data(v);
            v->items_p = temp;
        }
        else
        {
            for (i = v->used_size; i > index; --i)
            {
                *((v->items_p) + i) = *((v->items_p) + i - 1);
            }
            *((v->items_p) + i) = object;
            v->used_size++;
        }
    }
}

void vector_deep_copy(const Vector *v_src, Vector *v_des)
{ /*存在问题*/
    v_des->used_size = v_src->used_size;
    v_des->allocated_size = v_src->allocated_size;
    /* free(v_des->obj_funcs); */
    v_des->obj_funcs = v_src->obj_funcs;

    v_des->items_p = vector_calloc_data(v_des->allocated_size);
    assert(v_des->items_p);
    int i;
    for (i = 0; i < v_src->used_size; ++i)
    {
        /* *((v_des->items_p) + i) = *((v_src->items_p) + i); */
        *(v_des->items_p + i) = v_des->obj_funcs->p_func_object_alloc(v_des->obj_funcs->object_pointer_size); /*这里不用把结果转换成void* */
        v_des->obj_funcs->p_func_object_deepcopy(*((v_des->items_p) + i), *((v_src->items_p) + i));
    }
}

void **vector_calloc_data(size_type size)
{
    void **temp = (void **)calloc(size, sizeof(void *));
    assert(temp);
    return temp;
}

Vector *vector_slice(Vector *v, size_type start, size_type end, size_type step, ObjectFuncs *o)
{
    assert(v != NULL); /* 检查传入指针合法性 */
    if (step == 0)
    { /* 检查 step 不为0 */
        fprintf(stderr, "Illegal step!\n");
        return NULL;
    }
    if (start < -(v->used_size + 1) || start > (v->used_size) || end < -(v->used_size + 1) || end > (v->used_size))
    { /* 检查 索引越界 */
        fprintf(stderr, "Illegal index!\n");
        return NULL;
    }

    /* 将正负索引统一为正索引*/
    size_type positv_start = start >= 0 ? start : v->used_size + start;
    size_type positv_end = end >= 0 ? end : v->used_size + end;

    /* 索引先后和步长正负判断 */
    if ((positv_start < positv_end && step < 0) || (positv_start > positv_end && step > 0))
    {
        fprintf(stderr, "Step dose not match index's direction!\n");
        return NULL;
    }

    /* 求切片后的长度 */
    int new_length = abs(positv_end - positv_start) % abs(step) == 0 ? abs(positv_end - positv_start) / abs(step) : (positv_end - positv_start) / abs(step) + 1;
    if (new_length == 0)
    {
        fprintf(stderr, "Zero length!\n");
        return NULL;
    }

    /* 分配新vector */
    Vector *v_new = vector_new(new_length, o);
    assert(v_new != NULL);

    /* 赋值 */
    int i = 0;
    int j = 0;
    for (; i < new_length; i++)
    {
        *((v_new->items_p) + i) = *((v->items_p) + j + positv_start);
        j += step;
    }
    return v_new;
}

/* 这个size 是object的指针的size ,一般是4*/
void vector_reverse(Vector *v, size_type size)
{
    /*注意 *(items_p+i)是连续的, *(items_p+i)中的值不一定是连续的*/
    unsigned char *head = (unsigned char *)(v->items_p + 0);
    unsigned char *tail = (unsigned char *)(v->items_p + v->used_size - 1);
    unsigned char *temp = (unsigned char *)calloc(size, sizeof(unsigned char));
    while (head < tail)
    {

        memcpy(temp, head, size);
        memcpy(head, tail, size);
        memcpy(tail, temp, size);
        head += size;
        tail -= size;
    }
    free(temp);
    temp = NULL;
}
