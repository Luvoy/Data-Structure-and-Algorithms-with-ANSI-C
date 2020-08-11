#include "vector_ptype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static void **vector_ptype_calloc_data(vector_ptype_index_type size)
{ /* 开辟序列的内存空间，相当于为每个元素的指针开辟空间，但是并不为每个元素开辟空间*/
    void **temp = (void **)calloc(size, sizeof(void *));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(temp);
    return temp;
}

static void vector_ptype_free_data(VectorPtype *v)
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

extern VectorPtype *vector_ptype_new(const vector_ptype_index_type size)
{
    VectorPtype *v;
    assert(size > 0);
    v = (VectorPtype *)calloc(1, sizeof(VectorPtype));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(v);
    v->allocated_size = ((size / VECTOR_PTYPE_MAX_ALLOCATE_SIZE) + 1) * VECTOR_PTYPE_MAX_ALLOCATE_SIZE;
    v->used_size = size;
    v->items_p = vector_ptype_calloc_data(v->allocated_size);
    return v;
}

extern VectorPtype *vector_ptype_new_from_vector(const VectorPtype *v)
{ /*浅拷贝*/
    VectorPtype *v_new = vector_ptype_new(v->used_size);
#ifdef DEBUG_VECTOR_PTYPE
    fprintf(stderr, "In [vector_ptype_new_from_vector] new vector: OK\n");
#endif
    vector_ptype_index_type i;
    for (i = 0; i < v->used_size; ++i)
    {
        *(v_new->items_p + i) = *(v->items_p + i);
    }
    return v_new;
}

extern VectorPtype *vector_ptype_new_from_args(vector_ptype_index_type size, ...)
{
    assert(size > 0);
    VectorPtype *v_new = vector_ptype_new(size);
    assert(v_new != NULL);
#ifdef DEBUG_VECTOR_PTYPE
    fprintf(stderr, "In [vector_ptype_new_from_args] new vector: OK\n");
#endif
    va_list vl;
    va_start(vl, size);
    vector_ptype_index_type i;
    for (i = 0; i < size; ++i)
    {
        *(v_new->items_p + i) = va_arg(vl, void *);
    }
    va_end(vl);
    return v_new;
}

extern VectorPtype *vector_ptype_new_from_elements(vector_ptype_index_type size, void **p)
{
    VectorPtype *v = vector_ptype_new(size);
    assert(v != NULL);
#ifdef DEBUG_VECTOR_PTYPE
    fprintf(stderr, "In [vector_ptype_new_from_elements] new vector: OK\n");
#endif
    vector_ptype_index_type i;
    for (i = 0; i < size; ++i)
    {
        *(v->items_p + i) = *(p + i);
    }
    return v;
}

extern void vector_ptype_print(const VectorPtype *v, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *))
{
    if (v == NULL)
    {
        fprintf(fp, "NULL vector!\n");
        return;
    }
    vector_ptype_index_type i;
    if (strcmp(format, "SINGLE_LINE") == 0)
    {
        for (i = 0; i < v->used_size; ++i)
        {
            if (i == 0)
            {
                fprintf(fp, "[");
            }
            elem_print(fp, *((v->items_p) + i));
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
            elem_print(fp, *((v->items_p) + i));
            fprintf(fp, "\n");
        }
    }
    else
    {
        fprintf(fp, "Undefined format string: %s\n", format);
    }
    return;
}

extern void vector_ptype_free(VectorPtype **v)
{
    assert(v);
    if (*v == NULL)
    {
        return;
    }
    vector_ptype_free_data(*v);
#ifdef DEBUG_VECTOR_PTYPE
    fprintf(stderr, "In [vector_ptype_free] data: OK\n");
#endif
    free(*v);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *v = NULL;
}

extern vector_ptype_index_type vector_ptype_length(const VectorPtype *v)
{
    assert(v != NULL);
    return v->used_size;
}

extern void *vector_ptype_element_at(const VectorPtype *v, vector_ptype_index_type index, my_bool allow_negative_index)
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
        vector_ptype_index_type real_index = index > 0 ? index : v->used_size + index;
        return *((v->items_p) + real_index);
    }
}

extern void vector_ptype_index_assign(const VectorPtype *v, vector_ptype_index_type index, void *elem)
{
    assert(v != NULL);
    if (index < 0 || index >= v->used_size)
    {
        fprintf(stderr, "Illegal index!\n");
        return;
    }
    *((v->items_p) + index) = elem;
    return;
}

extern vector_ptype_index_type vector_ptype_left_search(const VectorPtype *v, void *key, my_bool (*elem_equal)(const void *, const void *))
{
    assert(v != NULL);
    vector_ptype_index_type i;
    for (i = 0; i < v->used_size; ++i)
    {
        if (elem_equal(key, *((v->items_p) + i)) == TRUE)
        {
            return i;
        }
    }
    return VECTOR_PTYPE_NOT_FOUND_INDEX;
}

extern void vector_ptype_append(VectorPtype *v, void *elem)
{
    assert(v != NULL);
    if (v->used_size < v->allocated_size)
    {
        *((v->items_p) + (v->used_size)) = elem;
        v->used_size++;
    }
    else
    {
        void **temp = vector_ptype_calloc_data(v->allocated_size + VECTOR_PTYPE_MAX_ALLOCATE_SIZE);
        assert(temp);
        vector_ptype_index_type i;
        for (i = 0; i < v->used_size; ++i)
        {
            *(temp + i) = *((v->items_p) + i);
        }
        *(temp + i) = elem;
        vector_ptype_free_data(v);
        v->used_size++;
        v->items_p = temp;
    }
}

extern void vector_ptype_join(VectorPtype *v_left, VectorPtype *v_right)
{ /*浅拷贝.连接之后， 当right删除、添加元素的时候，left不变. */
    assert(v_left != NULL);
    assert(v_right != NULL);

    if (v_left->used_size + v_right->used_size <= v_left->allocated_size)
    {
        vector_ptype_index_type i;
        for (i = 0; i < v_right->used_size; ++i)
        { /*把right的元素的地址拿过来*/
            *((v_left->items_p) + (v_left->used_size) + i) = *((v_right->items_p) + i);
        }
        v_left->used_size += v_right->used_size;

        /* vector_ptype_free(v_right); */
        /* 不应该free right */
    }
    else
    {
        void **temp = vector_ptype_calloc_data(VECTOR_PTYPE_MAX_ALLOCATE_SIZE * (((v_left->used_size) + (v_right->used_size)) / VECTOR_PTYPE_MAX_ALLOCATE_SIZE + 1));
        assert(temp);
        vector_ptype_index_type i;
        for (i = 0; i < v_left->used_size; ++i)
        {
            *(temp + i) = *((v_left->items_p) + i);
        }
        for (i = 0; i < v_right->used_size; ++i)
        {
            *(temp + (v_left->used_size) + i) = *((v_right->items_p) + i);
        }
        v_left->used_size += v_right->used_size;

        vector_ptype_free_data(v_left);
        v_left->items_p = temp;

        /* vector_ptype_free(v_right); */
        /* 不应该free right */
    }
}

extern void *vector_ptype_pop(VectorPtype *v)
{
    assert(v != NULL);
    if (v->used_size < 1)
    {
        fprintf(stderr, "No element to pop!\n");
        return NULL;
    }

    void *temp = *((v->items_p) + v->used_size - 1);
    *((v->items_p) + v->used_size - 1) = NULL;
    v->used_size--;
    return temp;
}

extern void *vector_ptype_pop_i(VectorPtype *v, vector_ptype_index_type index)
{
    assert(v != NULL);
    if (index < 0 || index > v->used_size - 1)
    {
        fprintf(stderr, "Illegal index!\n");
        return NULL;
    }
    vector_ptype_index_type i;
    void *ret = *((v->items_p) + index);
    for (i = index; i < v->used_size - 1; ++i)
    {
        *((v->items_p) + i) = *((v->items_p) + i + 1);
    }
    *((v->items_p) + i) = NULL;
    v->used_size--;
    return ret;
}

extern void vector_ptype_insert(VectorPtype *v, vector_ptype_index_type index, void *elem)
{
    assert(v != NULL);
    if (index < 0 || index > v->used_size)
    {
        fprintf(stderr, "Illegal index!\n");
        return;
    }
    if (index == v->used_size)
    {
        vector_ptype_append(v, elem);
        return;
    }
    else
    {
        vector_ptype_index_type i;
        if (v->used_size == v->allocated_size)
        {
            void **temp = vector_ptype_calloc_data(v->allocated_size + VECTOR_PTYPE_MAX_ALLOCATE_SIZE);
            assert(temp);
            for (i = 0; i < index; ++i)
            {
                *(temp + i) = *((v->items_p) + i);
            }
            *(temp + i) = elem;
            v->used_size++;
            for (++i; i < v->used_size; ++i)
            {
                *(temp + i) = *((v->items_p) + i - 1);
            }
            vector_ptype_free_data(v);
            v->items_p = temp;
        }
        else
        {
            for (i = v->used_size; i > index; --i)
            {
                *((v->items_p) + i) = *((v->items_p) + i - 1);
            }
            *((v->items_p) + i) = elem;
            v->used_size++;
        }
    }
}

extern VectorPtype *vector_ptype_slice(VectorPtype *v, vector_ptype_index_type start, vector_ptype_index_type end, vector_ptype_index_type step)
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
    vector_ptype_index_type positv_start = start >= 0 ? start : v->used_size + start;
    vector_ptype_index_type positv_end = end >= 0 ? end : v->used_size + end;

    /* 索引先后和步长正负判断 */
    if ((positv_start < positv_end && step < 0) || (positv_start > positv_end && step > 0))
    {
        fprintf(stderr, "Step dose not match index's direction!\n");
        return NULL;
    }

    /* 求切片后的长度 */
    vector_ptype_index_type new_length = abs(positv_end - positv_start) % abs(step) == 0 ? abs(positv_end - positv_start) / abs(step) : (positv_end - positv_start) / abs(step) + 1;
    if (new_length == 0)
    {
        fprintf(stderr, "Zero length!\n");
        return NULL;
    }

    /* 分配新vector */
    VectorPtype *v_new = vector_ptype_new(new_length);
    assert(v_new != NULL);

    /* 赋值 */
    vector_ptype_index_type i = 0;
    vector_ptype_index_type j = 0;
    for (; i < new_length; i++)
    {
        *((v_new->items_p) + i) = *((v->items_p) + j + positv_start);
        j += step;
    }
    return v_new;
}

extern void vector_ptype_reverse(VectorPtype *v)
{
    /*注意 *(items_p+i)是连续的, *(items_p+i)中的值不一定是连续的*/
    uint8_t *head = (uint8_t *)(v->items_p + 0);
    uint8_t *tail = (uint8_t *)(v->items_p + v->used_size - 1);
    uint8_t *temp = (uint8_t *)calloc(sizeof(void *), sizeof(uint8_t));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    while (head < tail)
    {

        memcpy(temp, head, sizeof(void *));
        memcpy(head, tail, sizeof(void *));
        memcpy(tail, temp, sizeof(void *));
        head += sizeof(void *);
        tail -= sizeof(void *);
    }
    free(temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    temp = NULL;
}
