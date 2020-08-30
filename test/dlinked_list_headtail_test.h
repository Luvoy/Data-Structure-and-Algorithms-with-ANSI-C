#include "../LinkedList/dlinked_list_headtail.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#ifndef _DLINKED_LIST_HEADTAIL_TEST_H_
#define _DLINKED_LIST_HEADTAIL_TEST_H_

extern void dlinked_list_headtail_test1(void);
extern void dlinked_list_headtail_test2(void);

#endif