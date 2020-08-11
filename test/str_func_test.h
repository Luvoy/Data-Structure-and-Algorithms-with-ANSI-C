#ifndef _STR_FUNC_TEST_H_
#define _STR_FUNC_TEST_H_

#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif

#include "../str_func.h"
extern void str_func_test();
#endif