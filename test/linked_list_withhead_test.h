#include "../LinkedList/linked_list_withhead.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#ifndef _LINKED_LIST_WITHHEAD_TEST_H_
#define _LINKED_LIST_WITHHEAD_TEST_H_

extern void linked_list_withhead_test_1(void);
extern void linked_list_withhead_test_2(void);

#endif