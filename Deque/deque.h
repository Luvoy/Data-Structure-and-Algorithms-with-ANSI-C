
#include "../my_bool.h"
#include "../my_debug.h"

#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#include <stdint.h>
#include <stdio.h>

#ifndef _DEQUE_H_
#define _DEQUE_H_

#ifndef _DEQUE_BUFFER_SIZE_TYPE_
#define _DEQUE_BUFFER_SIZE_TYPE_
typedef uint32_t deque_buffer_size_type;
#endif

#ifndef _DEQUE_MAP_SIZE_TYPE_
#define _DEQUE_MAP_SIZE_TYPE_
typedef uint32_t deque_map_size_type;
#endif

typedef void *T;

typedef struct DequeIterator
{
    T *cur;
    T *first;
    T *last;
    T **node;
} DequeIterator;

typedef struct Deque
{
    T **map_pointer;
    deque_buffer_size_type buffer_size;
    deque_map_size_type map_size;
    DequeIterator *head_iter_p;
    DequeIterator *tail_iter_p;

} Deque;

static deque_map_size_type inital_map_size = 8;
extern Deque *deque_init(size_t num_elems, deque_map_size_type num_buffers, deque_buffer_size_type buffer_size)
#endif
