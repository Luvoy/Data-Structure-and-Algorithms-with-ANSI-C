#include "../Vector/vector_generic.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif

#ifndef _VECTOR_GENERIC_TEST_H_
#define _VECTOR_GENERIC_TEST_H_

extern void vector_generic_test_1();
extern void vector_generic_test_2();

#endif