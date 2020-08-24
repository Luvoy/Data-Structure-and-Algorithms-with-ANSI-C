#include "linked_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static QNode *_qnode_new(void *elem)
{
    QNode *qn = (QNode *)malloc(sizeof(QNode));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(qn);
    qn->data = elem;
    qn->next = NULL;
    return qn;
}

extern my_bool linked_queue_is_empty(const LinkedQueue *lq)
{
    assert(lq != NULL);
    if (lq->head == lq->tail && lq->head == NULL)
    {
        return TRUE;
    }
    return FALSE;
}

extern linked_queue_size_type linked_queue_length(const LinkedQueue *lq)
{
    assert(lq);
    QNode *temp = lq->head;
    linked_queue_size_type len;
    for (len = 0; temp != NULL; temp = temp->next)
    {
        len++;
    }
    return len;
}

extern LinkedQueue *linked_queue_new_empty(void)
{
    LinkedQueue *lq = (LinkedQueue *)malloc(sizeof(LinkedQueue));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(lq);
    lq->head == NULL;
    lq->tail == NULL;
}

extern LinkedQueue *linked_queue_new_from_elements(linked_queue_size_type size, void **p)
{
    if (size < 0)
    {
        fprintf(stderr, "ERROR: negative size, expected size >= 0!\n");
        return NULL;
    }
    LinkedQueue *lq = (LinkedQueue *)malloc(sizeof(LinkedQueue));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(lq);
    lq->head == NULL;
    lq->tail == NULL;
    linked_queue_size_type i;
    QNode *move;
    for (i = 0; i < size; ++i)
    {
        QNode *temp = _qnode_new(*(p + i));
        if (i == 0)
        {
            lq->head = temp;
            lq->tail = temp;
        }
        else
        {
            lq->tail->next = temp;
            lq->tail = lq->tail->next;
        }
    }
    return lq;
}

extern LinkedQueue *linked_queue_new_from_args(linked_queue_size_type size, ...)
{
    if (size <= 0)
    {
        fprintf(stderr, "ERROR: expected size > 0!\n");
        return NULL;
    }
    LinkedQueue *lq = (LinkedQueue *)malloc(sizeof(LinkedQueue));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(lq != NULL);
    va_list vl;
    va_start(vl, size);
    linked_queue_size_type i;
    for (i = 0; i < size; ++i)
    {
        QNode *temp = _qnode_new(va_arg(vl, void *));
        if (i == 0)
        {
            lq->head = temp;
            lq->tail = temp;
        }
        lq->tail->next = temp;
        lq->tail = lq->tail->next;
    }
    va_end(vl);
    return lq;
}

extern void linked_queue_free(LinkedQueue **lqa)
{
    assert(lqa != NULL);
    assert((*lqa) != NULL);
    QNode *slow = (*lqa)->head;
    QNode *fast;
    for (; slow; slow = fast)
    {
        fast = slow ? slow->next : slow;
        free(slow);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
    }
    free(*lqa);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *lqa = NULL;
}

extern void linked_queue_clear(LinkedQueue *lq)
{
    assert(lq);
    while (!linked_queue_is_empty(lq))
    {
        linked_queue_pop(lq);
    }
}

extern void linked_queue_push(LinkedQueue *lq, const void *elem)
{
    assert(lq != NULL);
    QNode *temp = _qnode_new(elem);
    if (linked_queue_is_empty(lq))
    {
        lq->head = temp;
        lq->tail = temp;
    }
    else
    {
        lq->tail->next = temp;
        lq->tail = temp;
    }
}

extern void *linked_queue_pop(LinkedQueue *lq)
{
    assert(lq != NULL);
    if (linked_queue_is_empty(lq))
    {
        fprintf(stderr, "ERROR: empty queue!\n");
        return NULL;
    }
    else
    {
        void *ret = lq->head->data;
        QNode *temp = lq->head;
        if (lq->tail == lq->head)
        {
            lq->tail = NULL;
        }
        lq->head = lq->head->next;
        free(temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
        temp = NULL;
        return ret;
    }
}

extern void *linked_queue_get_head(const LinkedQueue *lq)
{
    assert(lq != NULL);
    if (linked_queue_is_empty(lq))
    {
        fprintf(stderr, "ERROR: empty queue!\n");
        return NULL;
    }
    return lq->head->data;
}

extern void linked_queue_print(const LinkedQueue *lq, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *))
{
    assert(lq != NULL);
    if (strcmp(format, "SINGLE_LINE") == 0)
    {
        fprintf(fp, "<<<< ");
        QNode *temp = lq->head;
        while (temp)
        {
            elem_print(fp, temp->data);
            temp = temp->next;
            fprintf(fp, " ");
        }
        fprintf(fp, "<<<<\n");
    }
    else if (strcmp(format, "MULTI_LINES") == 0)
    {
        QNode *temp = lq->head;
        while (temp)
        {
            fprintf(fp, "^ ");
            elem_print(fp, temp->data);
            temp = temp->next;
            fprintf(fp, "\n");
        }
    }
    else
    {
        fprintf(stderr, "ERROR: undefined format string: %s, expected SINGLE_LINE or MULTI_LINES.\n", format);
        return;
    }
}