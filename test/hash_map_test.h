#include "../HashMap/hash_map.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif

#ifndef HASH_MAP_TEST_H
#define HASH_MAP_TEST_H

void hash_map_test_1();
void hash_map_test_2();
void hash_map_test_3();

#endif