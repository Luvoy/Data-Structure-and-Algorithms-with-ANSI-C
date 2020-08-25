#include "../my_bool.h"
#include "../my_debug.h"

#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#include <stdint.h>
#include <stdio.h>

#ifndef _CYCLIC_QUEUE_H_
#define _CYCLIC_QUEUE_H_

#ifndef CYCLIC_QUEUE_SIZE_TYPE
#define CYCLIC_QUEUE_SIZE_TYPE
typedef uint32_t cyclic_queue_size_type;
#endif

typedef struct CyclicQueue
{ /* 实际上items的空间会空缺一个 */
    void **items;
    cyclic_queue_size_type allowed_capacity;
    cyclic_queue_size_type head;
    cyclic_queue_size_type tail; /*指向最后一个元素的末尾位置，而不是起始地址，也就是address+sizeof(void*) */
} CyclicQueue;

extern my_bool cyclic_queue_is_empty(const CyclicQueue *cq);
extern my_bool cyclic_queue_is_full(const CyclicQueue *cq);
extern cyclic_queue_size_type cyclic_queue_used_size(const CyclicQueue *cq);
extern cyclic_queue_size_type cyclic_queue_total_size(const CyclicQueue *cq);
extern CyclicQueue *cyclic_queue_new_empty(cyclic_queue_size_type max_size);
extern CyclicQueue *cyclic_queue_new_from_elements(cyclic_queue_size_type max_size, cyclic_queue_size_type elem_count, void **p);
extern CyclicQueue *cyclic_queue_new_from_args(cyclic_queue_size_type max_size, cyclic_queue_size_type elem_count, ...);
extern void cyclic_queue_free(CyclicQueue **cqa);
extern void cyclic_queue_clear(CyclicQueue *cq);
extern void cyclic_queue_push(CyclicQueue *cq, const void *elem);
extern void *cyclic_queue_pop(CyclicQueue *cq);
extern void *cyclic_queue_get_head(const CyclicQueue *cq);
extern void cyclic_queue_print(const CyclicQueue *cq, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *));

#endif