#include <stdio.h>
#include <stdlib.h>
#include "../test/hash_map_test.h"
#include "../test/linked_list_withhead_test.h"
#include "../test/str_func_test.h"
#include "../test/vector_ptype_test.h"
#include "../test/vector_generic_test.h"
#include "../test/sort_algo_test.h"
#include "../test/dlinked_list_headtail_test.h"

#ifdef DEBUG_ALLOC_FREE_COUNT
uint64_t g_alloc_count = 0;
uint64_t g_free_count = 0;
#endif
static void print_alloc_free_colorful(const char *func_name)
{
    if (g_alloc_count == g_free_count)
    {
        fprintf(stderr, "\033[1m\033[32mIn %s, alloc_count: %llu, free_count: %llu\033[0m\n", func_name, g_alloc_count, g_free_count);
    }
    else
    {
        fprintf(stderr, "\033[1m\033[31mIn %s, alloc_count: %llu, free_count: %llu\033[0m\n", func_name, g_alloc_count, g_free_count);
    }
}
int main(int argc, char const *argv[])
{
    str_func_test();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("str_func_test");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    vector_generic_test_1();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("vector_generic_test_1");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    vector_ptype_test_1();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("vector_ptype_test_1");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    linked_list_withhead_test_1();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("linked_list_withhead_test_1");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    hash_map_test_1();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("hash_map_test_1");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    hash_map_test_2();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("hash_map_test_2");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    hash_map_test_3();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("hash_map_test_3");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    sort_test_1();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("sort_test_1");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    sort_test_11();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("sort_test_11");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    sort_test_111();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("sort_test_111");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    sort_test_2();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("sort_test_2");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    sort_test_22();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("sort_test_22");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    sort_test_3();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("sort_test_3");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    sort_test_33();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("sort_test_33");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    sort_test_333();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("sort_test_333");
    g_alloc_count = 0;
    g_free_count = 0;
#endif
    sort_test_4();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("sort_test_4");
    g_alloc_count = 0;
    g_free_count = 0;
#endif

    dlinked_list_headtail_test1();
#ifdef DEBUG_ALLOC_FREE_COUNT
    print_alloc_free_colorful("dlinked_list_headtail_test1");
    g_alloc_count = 0;
    g_free_count = 0;
#endif

    return 0;
}
