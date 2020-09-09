#include "deque.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

extern Deque *deque_init(size_t num_elems, deque_map_size_type num_buffers, deque_buffer_size_type buffer_size)
{
    deque_map_size_type num_nodes = num_elems / (num_buffers * buffer_size) + 1;

    deque_map_size_type map_size = inital_map_size > num_nodes + 2 ? inital_map_size : num_nodes + 2;
}