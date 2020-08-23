#include "linked_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static QNode *_qnode_new(void *elem)
{
    QNode *qn = (QNode *)malloc(sizeof(QNode));
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
        }
    }
    return lq;
}

extern LinkedQueue *linked_queue_new_from_args(linked_queue_size_type size, ...)
{
}

extern void linked_queue_free(LinkedQueue **lqa)
{
}

extern void linked_queue_clear(LinkedQueue *lq)
{
}

extern void linked_queue_push(LinkedQueue *lq, const void *elem)
{
}

extern void *linked_queue_pop(LinkedQueue *lq)
{
}

extern void *linked_queue_get_head(const LinkedQueue *lq)
{
}

extern void linked_queue_print(const LinkedQueue *lq, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *))
{
}
