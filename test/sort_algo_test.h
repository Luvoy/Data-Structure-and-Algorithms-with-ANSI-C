#include "../SortAlgorithms/sort_algorithms.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#ifndef _SORT_ALGO_TEST_H_
#define _SORT_ALGO_TEST_H_

extern void sort_test_1();
extern void sort_test_11();
extern void sort_test_111();
extern void sort_test_2();
extern void sort_test_22();
extern void sort_test_3();
extern void sort_test_33();
extern void sort_test_333();
extern void sort_test_4();
#endif