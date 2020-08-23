#include "../my_bool.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#include <stdint.h>
#include <stdio.h>

#ifndef _STACK_PTYPE_H_
#define _STACK_PTYPE_H_

#ifndef STACK_SIZE_TYPE
#define STACK_SIZE_TYPE
typedef int64_t stack_size_type;
#endif

typedef struct Stack
{
    void **base;
    void **top;
    stack_size_type size;
} Stack;

extern my_bool stack_is_empty(const Stack *s);
extern my_bool stack_is_full(const Stack *s);
extern stack_size_type stack_used_size(const Stack *s);
extern stack_size_type stack_total_size(const Stack *s);
extern Stack *stack_new_empty(stack_size_type size);
extern Stack *stack_new_from_elements(stack_size_type stack_size, stack_size_type elem_count, void **p);
extern Stack *stack_new_from_args(stack_size_type stack_size, stack_size_type elem_count, ...);
extern void stack_free(Stack **sp);
extern void stack_clear(Stack *s);
extern void stack_push(Stack *s, const void *elem);
extern void *stack_pop(Stack *s);
extern void *stack_get_top(const Stack *s);
extern void stack_print(const Stack *s, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *));

#endif
