#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Vector *vector_new(const size_type size)
{
    Vector *v;
    if (size < 0)
    {
        return NULL;
    }
    v = (Vector *)calloc(1, sizeof(Vector));
    if (!v)
    {
        printf("Failed to allocate vector!\n");
        return NULL;
    }
    v->allocated_size = ((size / MAX_ALLOCATE_SIZE) + 1) * MAX_ALLOCATE_SIZE;
    v->used_size = size;
    v->items_p = (element_type *)calloc(v->allocated_size, sizeof(element_type));
    if (!v->items_p)
    {
        printf("Failed to allocate data area!\n");
        return NULL;
    }
    return v;
}

Vector *vector_new_from_vector(const Vector *v)
{
    Vector *v_new = vector_new(v->used_size);
    vector_copy(v, v_new);
    return v_new;
}

Vector *vector_new_from_elem_array(element_type *p, size_type size)
{
    Vector *v = vector_new(size);
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
        printf("NULL vector!\n");
        return;
    }
    int i;
    if (strcmp(format, "SINGLE_LINE") == 0)
    {
        for (i = 0; i < v->used_size; ++i)
        {
            if (i == 0)
            {
                printf("[");
            }
            if (i != v->used_size - 1)
            {
                printf("%d, ", *((v->items_p) + i));
            }
            else
            {
                printf("%d]\n", *((v->items_p) + i));
            }
        }
    }
    else if (strcmp(format, "MULTI_LINE") == 0)
    {
        for (i = 0; i < v->used_size; ++i)
        {
            printf("%d\n", *((v->items_p) + i));
        }
    }
    else
    {
        printf("Undefined format string!\n");
    }
    return;
}

void vector_free(Vector *v)
{
    if (v->items_p)
        free(v->items_p);
    free(v);
}

size_type vector_length(const Vector *v)
{
    if (v == NULL)
    {
        printf("NULL vector!\n");
        return ERROR_ELEMENT;
    }
    return v->used_size;
}

element_type vector_elem_at(const Vector *v, size_type index, my_bool allow_negative_index)
{
    if (v == NULL)
    {
        printf("NULL vector!\n");
        return ERROR_ELEMENT;
    }
    if (!allow_negative_index)
    {
        if (index < 0 || index >= v->used_size)
        {
            printf("Illegal index!\n");
            return ERROR_ELEMENT;
        }
        return *((v->items_p) + index);
    }
    else
    {
        if (index < -(v->used_size) || index >= v->used_size)
        {
            printf("Illegal index!\n");
            return ERROR_ELEMENT;
        }
        size_type real_index = index > 0 ? index : v->used_size + index;
        return *((v->items_p) + real_index);
    }
}

void vector_index_assign(const Vector *v, size_type index, element_type elem)
{
    if (v == NULL)
    {
        printf("NULL vector!\n");
        return;
    }
    if (index < 0 || index >= v->used_size)
    {
        printf("Illegal index!\n");
        return;
    }
    *((v->items_p) + index) = elem;
    return;
}

size_type vector_index_of(const Vector *v, element_type elem)
{
    if (v == NULL)
    {
        printf("NULL vector!\n");
        return ERROR_SIZE;
    }
    int i;
    for (i = 0; i < v->used_size; ++i)
    {
        if (compare(elem, *((v->items_p) + i) == 0))
        {
            return i;
        }
    }
    return ERROR_SIZE;
}

void vector_append(Vector *v, element_type elem)
{
    if (v == NULL)
    {
        printf("NULL vector!\n");
        return;
    }
    if (v->used_size < v->allocated_size)
    {
        *((v->items_p) + (v->used_size)) = elem;
        v->used_size++;
    }
    else
    {
        element_type *temp = (element_type *)calloc(v->allocated_size + MAX_ALLOCATE_SIZE, sizeof(element_type));
        if (!temp)
        {
            printf("calloc failed\n");
            return;
        }
        int i;
        for (i = 0; i < v->used_size; ++i)
        {
            *(temp + i) = *((v->items_p) + i);
        }
        *(temp + i) = elem;
        free(v->items_p);
        v->items_p = temp;
    }
}
void vector_join(Vector *v_left, Vector *v_right)
{
    if (v_left == NULL || v_right == NULL)
    {
        printf("NULL vector!\n");
        return;
    }
    if (v_left->used_size + v_right->used_size <= v_left->allocated_size)
    {
        int i;
        for (i = 0; i < v_right->used_size; ++i)
        {
            *((v_left->items_p) + (v_left->used_size) + i) = *((v_right->items_p) + i);
        }
        v_left->used_size += v_right->used_size;

        free(v_right->items_p);
        v_right->items_p = NULL;
        free(v_right);
        v_right = NULL; /* 可能不起作用 */
    }
    else
    {
        element_type *temp = (element_type *)calloc(MAX_ALLOCATE_SIZE * (((v_left->used_size) + (v_right->used_size)) / MAX_ALLOCATE_SIZE + 1), sizeof(element_type));
        if (!temp)
        {
            printf("calloc failed\n");
            return;
        }
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
        free(v_left->items_p);
        v_left->items_p = temp;

        free(v_right->items_p);
        v_right->items_p = NULL;
        free(v_right);
        v_right = NULL;
    }
}

element_type vector_pop(Vector *v)
{
    if (v == NULL)
    {
        printf("NULL vector!\n");
        return ERROR_ELEMENT;
    }
    if (v->used_size < 1)
    {
        printf("No element to pop!\n");
        return ERROR_ELEMENT;
    }

    v->used_size--;
    return *((v->items_p) + v->used_size);
}

element_type vector_pop_i(Vector *v, size_t index)
{
    if (v == NULL)
    {
        printf("NULL vector!\n");
        return ERROR_ELEMENT;
    }
    if (index < 0 || index > v->used_size - 1)
    {
        printf("Illegal index!\n");
        return ERROR_ELEMENT;
    }
    int i;
    element_type ret = *((v->items_p) + index);
    for (i = index; i < v->used_size - 1; ++i)
    {
        *((v->items_p) + i) = *((v->items_p) + i + 1);
    }
    *((v->items_p) + i) = 0;
    v->used_size--;
    return ret;
}

void vector_insert(Vector *v, size_t index, element_type elem)
{
    if (v == NULL)
    {
        printf("NULL vector!\n");
        return;
    }
    if (index < 0 || index > v->used_size)
    {
        printf("Illegal index!\n");
        return;
    }
    if (index == v->used_size)
    {
        vector_append(v, elem);
        return;
    }
    else
    {
        int i;
        if (v->used_size == v->allocated_size)
        {
            element_type *temp = (element_type *)calloc(v->allocated_size + MAX_ALLOCATE_SIZE, sizeof(element_type));
            if (!temp)
            {
                printf("Failed to allocate!\n");
                return;
            }
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
            free(v->items_p);
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

void vector_deep_copy(const Vector *v_src, Vector *v_des)
{
    v_des->used_size = v_src->used_size;
    v_des->allocated_size = v_src->allocated_size;
    free(v_des->items_p);
    v_des->items_p = (element_type *)calloc(v_des->allocated_size, sizeof(element_type));
    if (!(v_des->items_p))
    {
        printf("Failed to allocate!\n");
        return;
    }
    int i;
    for (i = 0; i < v_src->used_size; ++i)
    {
        *((v_des->items_p) + i) = *((v_src->items_p) + i);
    }
}

Vector *vector_slice(Vector *v, size_type start, size_type end, size_type step)
{
    if (v == NULL)
    { /* 检查传入指针合法性 */
        printf("NULL vector!\n");
        return NULL;
    }
    if (step == 0)
    { /* 检查 step 不为0 */
        printf("Illegal step!\n");
        return NULL;
    }
    if (start < -(v->used_size + 1) || start > (v->used_size) || end < -(v->used_size + 1) || end > (v->used_size))
    { /* 检查 索引越界 */
        printf("Illegal index!\n");
        return NULL;
    }

    /* 将正负索引统一为正索引*/
    size_type positv_start = start >= 0 ? start : v->used_size + start;
    size_type positv_end = end >= 0 ? end : v->used_size + end;

    /* 索引先后和步长正负判断 */
    if ((positv_start < positv_end && step < 0) || (positv_start > positv_end && step > 0))
    {
        printf("Step dose not match index's direction!\n");
        return NULL;
    }

    /* 求切片后的长度 */
    int new_length = abs(positv_end - positv_start) % abs(step) == 0 ? abs(positv_end - positv_start) / abs(step) : (positv_end - positv_start) / abs(step) + 1;
    if (new_length == 0)
    {
        printf("Zero length!\n");
        return NULL;
    }

    /* 分配新vector */
    Vector *v_new = (Vector *)calloc(1, sizeof(Vector));
    if (!v_new)
    {
        printf("Failed to allocate!\n");
        return NULL;
    }

    /* 赋值 */
    v_new->used_size = new_length;
    v_new->allocated_size = ((new_length / MAX_ALLOCATE_SIZE) + 1) * MAX_ALLOCATE_SIZE;
    v_new->items_p = (element_type *)calloc(v_new->used_size, sizeof(element_type));

    int i = 0;
    int j = 0;
    for (; i < new_length; i++)
    {
        *((v_new->items_p) + i) = *((v->items_p) + j + positv_start);
        j += step;
    }
    return v_new;
}

void vector_reverse(Vector *v)
{
    element_type *head = v->items_p;
    element_type *tail = v->items_p + v->used_size - 1;
    element_type temp;
    while (head < tail)
    {
        temp = *head;
        *head = *tail;
        *tail = temp;
        head++;
        tail--;
    }
}