#include "vector_generic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static void *vector_generic_calloc_data(vector_generic_index_type size)
{ /* 开辟序列的内存空间，相当于为每个元素的指针开辟空间，但是并不为每个元素开辟空间*/
    void *temp = (void *)calloc(size, sizeof(void));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(temp);
    return temp;
}

static void vector_generic_free_data(VectorGeneric *v)
{ /*释放序列的内存空间， 包括释放每个元素的内存空间*/
    if (v == NULL)
    {
        return;
    }
    if (v->items_p)
    {
        free(v->items_p);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
        v->items_p = NULL;
    }
}

extern VectorGeneric *vector_generic_new(const vector_generic_index_type size, elem_size_type elem_size)
{
    VectorGeneric *v;
    assert(size > 0);
    v = (VectorGeneric *)calloc(1, sizeof(VectorGeneric));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(v);
    v->allocated_size = ((size / VECTOR_GENERIC_MAX_ALLOCATE_SIZE) + 1) * VECTOR_GENERIC_MAX_ALLOCATE_SIZE;
    v->used_size = size;
    v->elem_size = elem_size;
    v->items_p = vector_generic_calloc_data(v->allocated_size * v->elem_size);
    ;

    return v;
}

extern VectorGeneric *vector_generic_new_from_vector(const VectorGeneric *v)
{
    VectorGeneric *v_new = vector_generic_new(v->used_size, v->elem_size);
#ifdef DEBUG_VECTOR_GENERIC
    fprintf(stderr, "In [vector_generic_new_from_vector] new vector: OK\n");
#endif
    vector_generic_index_type i;
    memcpy(v_new->items_p, v->items_p, v->used_size * v->elem_size);
    return v_new;
}

extern VectorGeneric *vector_generic_new_from_args(vector_generic_index_type size, elem_size_type elem_size, ...)
{
    assert(size > 0);
    VectorGeneric *v_new = vector_generic_new(size, elem_size);
    assert(v_new != NULL);
#ifdef DEBUG_VECTOR_GENERIC
    fprintf(stderr, "In [vector_generic_new_from_args] new vector: OK\n");
#endif
    va_list vl;
    va_start(vl, size);
    vector_generic_index_type i;
    for (i = 0; i < size; ++i)
    {
        void *temp = va_arg(vl, void *);
#ifdef DEBUG_VECTOR_GENERIC
        fprintf(stderr, "args[%d] is %p\n", i, temp);
#endif
        memcpy(v_new->items_p + i * elem_size, temp, elem_size);
    }
    va_end(vl);
    return v_new;
}

extern VectorGeneric *vector_generic_new_from_elements(vector_generic_index_type size, elem_size_type elem_size, void *p)
{
    VectorGeneric *v = vector_generic_new(size, elem_size);
    assert(v != NULL);
#ifdef DEBUG_VECTOR_GENERIC
    fprintf(stderr, "In [vector_generic_new_from_elements] new vector: OK\n");
#endif
    memcpy(v->items_p, p, size * elem_size);
    return v;
}

extern void vector_generic_print(const VectorGeneric *v, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *))
{
    if (v == NULL)
    {
        fprintf(fp, "NULL vector!\n");
        return;
    }
    vector_generic_index_type i;
    if (strcmp(format, "SINGLE_LINE") == 0)
    {
        for (i = 0; i < v->used_size; ++i)
        {
            if (i == 0)
            {
                fprintf(fp, "[");
            }
            elem_print(fp, (v->items_p) + i * v->elem_size);
            if (i != v->used_size - 1)
            {

                fprintf(fp, ", ");
            }
            else
            {
                fprintf(fp, "]\n");
            }
        }
    }
    else if (strcmp(format, "MULTI_LINES") == 0)
    {
        for (i = 0; i < v->used_size; ++i)
        {
            elem_print(fp, (v->items_p) + i * v->elem_size);
            fprintf(fp, "\n");
        }
    }
    else
    {
        fprintf(fp, "Undefined format string: %s\n", format);
    }
    return;
}

extern void vector_generic_free(VectorGeneric **v)
{
    assert(v);
    if (*v == NULL)
    {
        return;
    }
    vector_generic_free_data(*v);
#ifdef DEBUG_VECTOR_GENERIC
    fprintf(stderr, "In [vector_generic_free] data: OK\n");
#endif
    free(*v);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *v = NULL;
}

extern vector_generic_index_type vector_generic_length(const VectorGeneric *v)
{
    assert(v != NULL);
    return v->used_size;
}

extern void *vector_generic_element_at(const VectorGeneric *v, vector_generic_index_type index, my_bool allow_negative_index)
{
    assert(v != NULL);
    if (!allow_negative_index)
    {
        if (index < 0 || index >= v->used_size)
        {
            fprintf(stderr, "Illegal index!\n");
            return NULL;
        }
        return (v->items_p) + index * v->elem_size;
    }
    else
    {
        if (index < -(v->used_size) || index >= v->used_size)
        {
            fprintf(stderr, "Illegal index!\n");
            return NULL;
        }
        vector_generic_index_type real_index = index > 0 ? index : v->used_size + index;
        return (v->items_p) + real_index * v->elem_size;
    }
}

extern void vector_generic_index_assign(const VectorGeneric *v, vector_generic_index_type index, void *elem)
{
    assert(v != NULL);
    if (index < 0 || index >= v->used_size)
    {
        fprintf(stderr, "Illegal index!\n");
        return;
    }
    memcpy((v->items_p) + index * v->elem_size, elem, v->elem_size);
    return;
}

extern vector_generic_index_type vector_generic_left_search(const VectorGeneric *v, void *key, my_bool (*elem_equal)(const void *, const void *))
{
    assert(v != NULL);
    vector_generic_index_type i;
    for (i = 0; i < v->used_size; ++i)
    {
        if (elem_equal(v->items_p + i * v->elem_size, key) == TRUE)
        {
            return i;
        }
    }
    return VECTOR_GENERIC_NOT_FOUND_INDEX;
}

extern void vector_generic_append(VectorGeneric *v, void *elem)
{
    assert(v != NULL);
    if (v->used_size < v->allocated_size)
    {
        memcpy(v->items_p + v->used_size * v->elem_size, elem, v->elem_size);
        v->used_size++;
    }
    else
    {
        void *temp = vector_generic_calloc_data((v->allocated_size + VECTOR_GENERIC_MAX_ALLOCATE_SIZE) * v->elem_size);
        assert(temp);
        memcpy(temp, v->items_p, v->used_size * v->elem_size);
        memcpy(temp + v->used_size * v->elem_size, elem, v->elem_size);
        vector_generic_free_data(v);
        v->used_size++;
        v->items_p = temp;
    }
}

extern void vector_generic_join(VectorGeneric *v_left, VectorGeneric *v_right)
{ /*深拷贝.连接之后， left和right彻底无关 */
    assert(v_left != NULL);
    assert(v_right != NULL);

    if (v_left->used_size + v_right->used_size <= v_left->allocated_size)
    {
        memcpy(v_left->items_p + v_left->used_size * v_left->elem_size, v_right->items_p, v_right->used_size * v_right->elem_size);
        v_left->used_size += v_right->used_size;
    }
    else
    {
        void *temp = vector_generic_calloc_data(v_left->elem_size * (VECTOR_GENERIC_MAX_ALLOCATE_SIZE * (((v_left->used_size) + (v_right->used_size)) / VECTOR_GENERIC_MAX_ALLOCATE_SIZE + 1)));
        assert(temp);
        memcpy(temp, v_left->items_p, v_left->used_size * v_left->elem_size);
        memcpy(temp + v_left->used_size * v_left->elem_size, v_right->items_p, v_right->used_size * v_right->elem_size);
        vector_generic_free_data(v_left);
        v_left->items_p = temp;
        v_left->allocated_size = (VECTOR_GENERIC_MAX_ALLOCATE_SIZE * (((v_left->used_size) + (v_right->used_size)) / VECTOR_GENERIC_MAX_ALLOCATE_SIZE + 1));
        v_left->used_size = v_left->used_size + v_right->used_size;
    }
}

extern void *vector_generic_pop(VectorGeneric *v)
{ /*调用者需要free*/
    assert(v != NULL);
    if (v->used_size < 1)
    {
        fprintf(stderr, "No element to pop!\n");
        return NULL;
    }

    void *temp = malloc(v->elem_size);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(temp);
    memcpy(temp, (v->items_p) + v->elem_size * (v->used_size - 1), v->elem_size);
    memset((v->items_p) + v->elem_size * (v->used_size - 1), 0, v->elem_size);
    v->used_size--;
    return temp;
}

extern void *vector_generic_pop_i(VectorGeneric *v, vector_generic_index_type index)
{ /*调用者需要free*/
    assert(v != NULL);
    if (index < 0 || index > v->used_size - 1)
    {
        fprintf(stderr, "Illegal index!\n");
        return NULL;
    }
    vector_generic_index_type i;
    void *ret = malloc(v->elem_size);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(ret);
    memcpy(ret, v->items_p + index * v->elem_size, v->elem_size);
    for (i = index; i < v->used_size - 1; ++i)
    {
        memcpy(v->items_p + i * v->elem_size, v->items_p + (i + 1) * v->elem_size, v->elem_size);
    }
    memset(v->items_p + i * v->elem_size, 0, v->elem_size);
    v->used_size--;
    return ret;
}

extern void vector_generic_insert(VectorGeneric *v, vector_generic_index_type index, void *elem)
{
    assert(v != NULL);
    if (index < 0 || index > v->used_size)
    {
        fprintf(stderr, "Illegal index!\n");
        return;
    }
    if (index == v->used_size)
    {
        vector_generic_append(v, elem);
        return;
    }
    else
    {
        vector_generic_index_type i;
        if (v->used_size == v->allocated_size)
        {
            void *temp = vector_generic_calloc_data((v->allocated_size + VECTOR_GENERIC_MAX_ALLOCATE_SIZE) * v->elem_size);
            assert(temp);
            memcpy(temp, v->items_p, v->elem_size * index);
            memcpy(temp + v->elem_size * index, elem, v->elem_size);
            memcpy(temp + v->elem_size * (index + 1), v->items_p + v->elem_size * index, v->elem_size * (v->used_size - index));
            v->used_size++;
            vector_generic_free_data(v);
            v->items_p = temp;
        }
        else
        {

            for (i = v->used_size; i > index; --i)
            {
                memcpy(v->items_p + i * v->elem_size, v->items_p + (i - 1) * v->elem_size, v->elem_size);
            }
            memcpy(v->items_p + v->elem_size * i, elem, v->elem_size);
            v->used_size++;
        }
    }
}

extern VectorGeneric *vector_generic_slice(VectorGeneric *v, vector_generic_index_type start, vector_generic_index_type end, vector_generic_index_type step)
{                      /*切片， 支持负索引。类似于python。调用者必须free*/
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
    vector_generic_index_type positv_start = start >= 0 ? start : v->used_size + start;
    vector_generic_index_type positv_end = end >= 0 ? end : v->used_size + end;

    /* 索引先后和步长正负判断 */
    if ((positv_start < positv_end && step < 0) || (positv_start > positv_end && step > 0))
    {
        fprintf(stderr, "Step dose not match index's direction!\n");
        return NULL;
    }

    /* 求切片后的长度 */
    vector_generic_index_type new_length = abs(positv_end - positv_start) % abs(step) == 0 ? abs(positv_end - positv_start) / abs(step) : (positv_end - positv_start) / abs(step) + 1;
    if (new_length == 0)
    {
        fprintf(stderr, "Zero length!\n");
        return NULL;
    }

    /* 分配新vector */
    VectorGeneric *v_new = vector_generic_new(new_length, v->elem_size);
    assert(v_new != NULL);

    /* 赋值 */
    vector_generic_index_type i = 0;
    vector_generic_index_type j = 0;
    for (; i < new_length; ++i)
    {
        memcpy(v_new->items_p + i * v_new->elem_size, v->items_p + v->elem_size * (j + positv_start), v->elem_size);
        j += step;
    }
    return v_new;
}

extern void vector_generic_reverse(VectorGeneric *v)
{
    /*注意 *(items_p+i)是连续的, *(items_p+i)中的值不一定是连续的*/
    uint8_t *head = (uint8_t *)(v->items_p + 0);
    uint8_t *tail = (uint8_t *)(v->items_p + v->elem_size * (v->used_size - 1));
    uint8_t *temp = (uint8_t *)calloc(v->elem_size, sizeof(uint8_t));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    while (head < tail)
    {

        memcpy(temp, head, v->elem_size);
        memcpy(head, tail, v->elem_size);
        memcpy(tail, temp, v->elem_size);
        head += v->elem_size;
        tail -= v->elem_size;
    }
    free(temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    temp = NULL;
}
