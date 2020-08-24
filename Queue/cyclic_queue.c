#include "cyclic_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

extern my_bool cyclic_queue_is_empty(const CyclicQueue *cq)
{
    assert(cq != NULL);
    if (cq->head == cq->tail)
    {
        return TRUE;
    }
    return FALSE;
}

extern my_bool cyclic_queue_is_full(const CyclicQueue *cq)
{
    return (my_bool)(cyclic_queue_used_size == cyclic_queue_total_size);
}

extern cyclic_queue_size_type cyclic_queue_used_size(const CyclicQueue *cq)
{
    assert(cq != NULL);
    return (cq->tail - cq->head + cq->allowed_capacity + 1) % (cq->allowed_capacity + 1);
}

extern cyclic_queue_size_type cyclic_queue_total_size(const CyclicQueue *cq)
{
    assert(cq != NULL);
    return cq->allowed_capacity;
}

extern CyclicQueue *cyclic_queue_new_empty(cyclic_queue_size_type max_size)
{ /* if max_size==0, the CQ returned is both empty and full */
    if (max_size < 0)
    {
        fprintf(stderr, "ERROR: expected max_size >= 0!\n");
        return NULL;
    }
    CyclicQueue *cq = (CyclicQueue *)malloc(sizeof(CyclicQueue));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(cq);
    cq->allowed_capacity = max_size;
    cq->items = (void **)calloc(max_size + 1, sizeof(void *));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    cq->head = 0;
    cq->tail = cq->head;
    return cq;
}

extern CyclicQueue *cyclic_queue_new_from_elements(cyclic_queue_size_type max_size, cyclic_queue_size_type elem_count, void **p)
{ /* if max_size==0, the CQ returned is both empty and full */
    if (max_size < 0)
    {
        fprintf(stderr, "ERROR: expected max_size >= 0!\n");
        return NULL;
    }
    if (max_size < elem_count)
    {
        fprintf(stderr, "ERROR: expected max_size >= elem_count!\n");
    }
    CyclicQueue *cq = (CyclicQueue *)malloc(sizeof(CyclicQueue));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(cq);
    cq->allowed_capacity = max_size;
    cq->items = (void **)calloc(max_size + 1, sizeof(void *));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    cq->head = 0;
    cq->tail = cq->head;
    cyclic_queue_size_type i;
    for (i = 0; i < elem_count; ++i)
    {
        *(cq->items + i) = *(p + i);
        cq->tail++;
    }
    return cq;
}

extern CyclicQueue *cyclic_queue_new_from_args(cyclic_queue_size_type max_size, cyclic_queue_size_type elem_count, ...)
{
    if (elem_count <= 0)
    {
        fprintf(stderr, "ERROR: expected elem_count >= 0!\n");
        return NULL;
    }
    if (max_size < elem_count)
    {
        fprintf(stderr, "ERROR: expected max_size >= elem_count!\n");
    }
    CyclicQueue *cq = (CyclicQueue *)malloc(sizeof(CyclicQueue));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(cq);
    cq->allowed_capacity = max_size;
    cq->items = (void **)calloc(max_size + 1, sizeof(void *));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    cq->head = 0;
    cq->tail = cq->head;
    va_list vl;
    va_start(vl, elem_count);
    cyclic_queue_size_type i;
    for (i = 0; i < elem_count; ++i)
    {
        *(cq->items + i) = va_arg(vl, void *);
        cq->tail++;
    }
    va_end(vl);
    return cq;
}

extern void cyclic_queue_free(CyclicQueue **cqa)
{
    assert(cqa != NULL);
    assert(*cqa != NULL);
    free((*cqa)->items);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(*cqa);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *cqa = NULL;
}

extern void cyclic_queue_clear(CyclicQueue *cq)
{
    assert(cq != NULL);
    while (!cyclic_queue_is_empty(cq))
    {
        cyclic_queue_pop(cq);
    }
}

extern void cyclic_queue_push(CyclicQueue *cq, const void *elem)
{
    assert(cq != NULL);
    if (cyclic_queue_is_full(cq))
    {
        fprintf(stderr, "ERROR: cylic_queue is full!\n");
        return;
    }
    else
    {
        *(cq->items + (cq->tail)) = elem;
        cq->tail = (cq->tail + 1) % (cq->allowed_capacity + 1);
    }
}

extern void *cyclic_queue_pop(CyclicQueue *cq)
{
    assert(cq != NULL);
    if (cyclic_queue_is_empty(cq))
    {
        fprintf(stderr, "ERROR: cylic_queue is empty!\n");
        return NULL;
    }
    else
    {
        void *ret = *(cq->items + cq->head);
        memset(*(cq->items + cq->head), 0, sizeof(void *));
        cq->head = (cq->head + 1) % (cq->allowed_capacity + 1);
        return ret;
    }
}

extern void *cyclic_queue_get_head(const CyclicQueue *cq)
{
    assert(cq != NULL);
    if (cyclic_queue_is_empty(cq))
    {
        fprintf(stderr, "ERROR: cylic_queue is empty!\n");
        return NULL;
    }
    else
    {
        return *(cq->items + cq->head);
    }
}

extern void cyclic_queue_print(const CyclicQueue *cq, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *))
{
    assert(cq != NULL);
    if (strcmp(format, "SINGLE_LINE") == 0)
    {
        fprintf(fp, "<<<< ");
        cyclic_queue_size_type i;
        for (i = 0; i < cq->allowed_capacity; ++i)
        {
            if (*(cq->items + i) == NULL)
            {
                fprintf(fp, "[]");
            }
            else
            {
                elem_print(fp, *(cq->items + i));
            }
            if (i == cq->head)
            {
                fprintf(fp, "(head)");
            }
            if (i == cq->tail)
            {
                fprintf(fp, "(tail)");
            }
            if (i != cq->allowed_capacity - 1)
            {
                fprintf(fp, " <<");
            }
        }
        fprintf(fp, " <<<<\n");
    }
    else if (strcmp(format, "MULTI_LINES") == 0)
    {
        cyclic_queue_size_type i;
        for (i = 0; i < cq->allowed_capacity; ++i)
        {
            fprintf(fp, "^ ");
            if (*(cq->items + i) == NULL)
            {
                fprintf(fp, "[]");
            }
            else
            {
                elem_print(fp, *(cq->items + i));
            }
            if (i == cq->head)
            {
                fprintf(fp, "(head)");
            }
            if (i == cq->tail)
            {
                fprintf(fp, "(tail)");
            }
            fprintf(fp, "\n");
        }
    }
    else
    {
        fprintf(stderr, "ERROR: undefined format string: %s, expected SINGLE_LINE or MULTI_LINES.\n", format);
        return;
    }
}