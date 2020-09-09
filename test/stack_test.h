#include "../Stack/stack_ptype.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif

#ifndef _STACK_TEST_H_
#define _STACK_TEST_H_

extern void stack_test_1();

#endif