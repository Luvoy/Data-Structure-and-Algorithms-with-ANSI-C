#include "../my_bool.h"
#include "../my_debug.h"

#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#include <stdint.h>
#include <stdio.h>

#ifndef _LINKED_QUEUE_H_
#define _LINKED_QUEUE_H_

#ifndef LINKED_QUEUE_SIZE_TYPE
#define LINKED_QUEUE_SIZE_TYPE
typedef uint32_t linked_queue_size_type;
#endif

typedef struct QNode
{
    void *data;
    struct QNode *next;
} QNode;

typedef struct LinkedQueue
{
    QNode *head;
    QNode *tail; /*指向最后一个元素的起始地址 */
} LinkedQueue;

extern my_bool linked_queue_is_empty(const LinkedQueue *lq);
extern linked_queue_size_type linked_queue_length(const LinkedQueue *lq);
extern LinkedQueue *linked_queue_new_empty(void);
extern LinkedQueue *linked_queue_new_from_elements(linked_queue_size_type size, void **p);
extern LinkedQueue *linked_queue_new_from_args(linked_queue_size_type size, ...);
extern void linked_queue_free(LinkedQueue **lqa);
extern void linked_queue_clear(LinkedQueue *lq);
extern void linked_queue_push(LinkedQueue *lq, const void *elem);
extern void *linked_queue_pop(LinkedQueue *lq);
extern void *linked_queue_get_head(const LinkedQueue *lq);
extern void linked_queue_print(const LinkedQueue *lq, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *));

#endif