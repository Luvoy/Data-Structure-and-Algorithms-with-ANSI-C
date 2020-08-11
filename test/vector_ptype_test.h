#include "../Vector/vector_ptype.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif

#ifndef _VECTOR_PTYPE_TEST_H_
#define _VECTOR_PTYPE_TEST_H_

extern void vector_ptype_test_1();
extern void vector_ptype_test_2();

#endif