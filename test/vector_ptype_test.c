#include "vector_ptype_test.h"
#include <stdio.h>
#include <assert.h>
typedef int elem_type;
typedef VectorPtype VectorIntP;
static void elem_print(FILE *fp, const void *elem)
{
    assert(elem);
    fprintf(fp, "%d", *((const elem_type *)elem));
}
static my_bool elem_equal(const void *e1, const void *e2)
{
    if (*((const elem_type *)e1) - *((const elem_type *)e2) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

extern void vector_ptype_test_1()
{
    fprintf(stdout, "Generate data:\n");
    elem_type *a1 = calloc(1, sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    elem_type *a2 = calloc(1, sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    elem_type *a3 = calloc(1, sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    elem_type *a4 = calloc(1, sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    elem_type *a5 = calloc(1, sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    elem_type *a6 = calloc(1, sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    elem_type *a7 = calloc(1, sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    elem_type *a8 = calloc(1, sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    elem_type *a9 = calloc(1, sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    elem_type *a10 = calloc(1, sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif

    *(a1 + 0) = 1;
    *(a2 + 0) = 2;
    *(a3 + 0) = 3;
    *(a4 + 0) = 4;
    *(a5 + 0) = 5;
    *(a6 + 0) = 6;
    *(a7 + 0) = 7;
    *(a8 + 0) = 8;
    *(a9 + 0) = 9;
    *(a10 + 0) = 10;

    elem_type _a11[] = {11};
    elem_type _a12[] = {12};
    elem_type _a13[] = {13};
    elem_type _a14[] = {14};
    elem_type _a15[] = {15};
    elem_type _a16[] = {16};
    elem_type _a17[] = {17};
    elem_type _a18[] = {18};
    elem_type _a19[] = {19};
    elem_type _a20[] = {20};
    elem_type _a21[] = {21};
    elem_type *data[] = {a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, _a11, _a12, _a13, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21};
    fprintf(stdout, "Generate data: OK\n");
    fprintf(stdout, "Test vector_ptype_new:\n");
    fprintf(stdout, "Please input the size of vector (1-10)\n");
    vector_ptype_index_type n = 6;
    VectorIntP *vi1 = vector_ptype_new(n);
    fprintf(stdout, "create vector: OK\n");
    fprintf(stdout, "Test vector_ptype_index_assign:\n");
    elem_type i;
    for (i = 0; i < n; ++i)
    {
        fprintf(stdout, "assigning %d...\n", i);
        vector_ptype_index_assign(vi1, i, data[i]);
    }
    fprintf(stdout, "assign complete\n");
    fprintf(stdout, "Test vector_ptype_length:\n");
    fprintf(stdout, "vi1 len: %d\n", vector_ptype_length(vi1));
    fprintf(stdout, "Test print:\n");
    vector_ptype_print(vi1, stdout, "SINGLE_LINE", elem_print);
    fprintf(stdout, "print over.\n");

    fprintf(stdout, "Test vector_ptype_new_from_elements:\n");
    VectorIntP *vi2 = vector_ptype_new_from_elements(7, (void **)(data + n));
    fprintf(stdout, "vi2:\n");
    vector_ptype_print(vi2, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_ptype_new_from_args:\n");
    VectorIntP *vi3 = vector_ptype_new_from_args(8, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21);
    fprintf(stdout, "vi3:\n");
    vector_ptype_print(vi3, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_ptype_new_from_vector:\n");
    VectorIntP *vi4 = vector_ptype_new_from_vector(vi1);
    fprintf(stdout, "vi4:\n");
    vector_ptype_print(vi4, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_ptype_length:\n");
    fprintf(stdout, "vi2 vector_ptype_length: %d\n", vector_ptype_length(vi2));
    fprintf(stdout, "vi3 vector_ptype_length: %d\n", vector_ptype_length(vi3));
    fprintf(stdout, "vi4 vector_ptype_length: %d\n", vector_ptype_length(vi4));

    fprintf(stdout, "Test vector_ptype_element_at:\n");
    fprintf(stdout, "vi2[1] is ");
    elem_print(stdout, vector_ptype_element_at(vi2, 1, FALSE));
    fprintf(stdout, "\n");

    fprintf(stdout, "vi3[-2] is ");
    elem_print(stdout, vector_ptype_element_at(vi3, -2, TRUE));
    fprintf(stdout, "\n");

    fprintf(stdout, "Test vector_ptype_left_search:\n");

    elem_type a_temp_1[] = {3};
    elem_type a_temp_2[] = {8};
    fprintf(stdout, "try to find ");
    elem_print(stdout, a_temp_1);
    fprintf(stdout, " in vi1:\n");
    fprintf(stdout, "%d\n", vector_ptype_left_search(vi1, a_temp_1, elem_equal));
    fprintf(stdout, "try to find ");
    elem_print(stdout, a_temp_2);
    fprintf(stdout, " in vi2:\n");
    fprintf(stdout, "%d\n", vector_ptype_left_search(vi2, a_temp_2, elem_equal));

    fprintf(stdout, "Test vector_ptype_append:\nBefore append, vi2:\n");
    vector_ptype_print(vi2, stdout, "SINGLE_LINE", elem_print);
    vector_ptype_append(vi2, a_temp_1);
    fprintf(stdout, "after append ");
    elem_print(stdout, a_temp_1);
    fprintf(stdout, " to vi2, vi2 :\n ");
    vector_ptype_print(vi2, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_ptype_pop:\nBefore pop, vi4:\n");
    vector_ptype_print(vi4, stdout, "SINGLE_LINE", elem_print);
    void *pop_ret;
    pop_ret = vector_ptype_pop(vi4);
    fprintf(stdout, "after pop, vi4:\n");
    vector_ptype_print(vi4, stdout, "SINGLE_LINE", elem_print);
    fprintf(stdout, "poped elem:");
    elem_print(stdout, pop_ret);
    /*  free(pop_ret); */ /*调用者无需free*/
                          /* #ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif */

    fprintf(stdout, "\nTest vector_ptype_pop_i:\n");
    pop_ret = vector_ptype_pop_i(vi4, 2);
    fprintf(stdout, "after pop_2, vi4:\n");
    vector_ptype_print(vi4, stdout, "SINGLE_LINE", elem_print);
    fprintf(stdout, "poped elem:");
    elem_print(stdout, pop_ret);
    /*     free(pop_ret);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif */

    fprintf(stdout, "Test vector_ptype_insert:\n");
    vector_ptype_insert(vi4, 1, a_temp_2);
    fprintf(stdout, "after insert ");
    elem_print(stdout, a_temp_2);
    fprintf(stdout, " to vi4 at location 1 :\n");
    vector_ptype_print(vi4, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_ptype_slice:\n");
    VectorIntP *vi_temp = NULL;
    fprintf(stdout, "vi2, 0, 0, 1, is ");
    vi_temp = vector_ptype_slice(vi2, 0, 0, 1);
    vector_ptype_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    if (vi_temp != NULL)
    {
        vector_ptype_free(&vi_temp);
    }
    fprintf(stdout, "vi2, -1, -1, 1, is ");
    vi_temp = vector_ptype_slice(vi2, -1, -1, 1);
    vector_ptype_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    if (vi_temp != NULL)
    {
        vector_ptype_free(&vi_temp);
    }
    fprintf(stdout, "vi2, 1, 1, -1, is ");
    vi_temp = vector_ptype_slice(vi2, 1, 1, -1);
    vector_ptype_print(vi_temp, stdout, "SINGLE_LINE", elem_print);

    if (vi_temp != NULL)
    {
        vector_ptype_free(&vi_temp);
    }

    fprintf(stdout, "vi2, -1, -1, -1, is ");
    vi_temp = vector_ptype_slice(vi2, -1, -1, -1);
    vector_ptype_print(vi_temp, stdout, "SINGLE_LINE", elem_print);

    if (vi_temp != NULL)
    {
        vector_ptype_free(&vi_temp);
    }

    fprintf(stdout, "vi2, -4, -1, -2, is ");
    vi_temp = vector_ptype_slice(vi2, -4, -1, -2);
    vector_ptype_print(vi_temp, stdout, "SINGLE_LINE", elem_print);

    if (vi_temp != NULL)
    {
        vector_ptype_free(&vi_temp);
    }

    fprintf(stdout, "vi2, -3, -1, 1, is ");
    vi_temp = vector_ptype_slice(vi2, -3, -1, 1);
    vector_ptype_print(vi_temp, stdout, "SINGLE_LINE", elem_print);

    if (vi_temp != NULL)
    {
        vector_ptype_free(&vi_temp);
    }

    fprintf(stdout, "vi2, -1,-5, -1, is ");
    vi_temp = vector_ptype_slice(vi2, -1, -5, -1);
    vector_ptype_print(vi_temp, stdout, "SINGLE_LINE", elem_print);

    if (vi_temp != NULL)
    {
        vector_ptype_free(&vi_temp);
    }

    fprintf(stdout, "vi2, 4,2, -2, is ");
    vi_temp = vector_ptype_slice(vi2, 4, 2, -2);
    vector_ptype_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    if (vi_temp != NULL)
    {
        vector_ptype_free(&vi_temp);
    }

    fprintf(stdout, "Test vector_ptype_reverse:\nBefore reverse, vi2:\n");
    vector_ptype_print(vi2, stdout, "SINGLE_LINE", elem_print);
    vector_ptype_reverse(vi2);
    vector_ptype_print(vi2, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_ptype_join:\nBefore join, vi3:\n");
    vector_ptype_print(vi3, stdout, "SINGLE_LINE", elem_print);
    vector_ptype_join(vi3, vi4);
    fprintf(stdout, "after join(vi3, vi4), vi3:\n");
    vector_ptype_print(vi3, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test free:\n");
    vector_ptype_free(&vi1);
    vector_ptype_free(&vi2);
    vector_ptype_free(&vi3);
    vector_ptype_free(&vi4);
    fprintf(stdout, "free over\n");

    fprintf(stdout, "Free original data:\n");
    free(a1);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "free over\n");
    free(a2);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "free over\n");
    free(a3);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "free over\n");
    free(a4);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "free over\n");
    free(a5);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "free over\n");
    free(a6);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "free over\n");
    free(a7);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "free over\n");
    free(a8);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "free over\n");
    free(a9);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "free over\n");
    free(a10);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "free over\n");
    fprintf(stdout, "Free original data: OK\n");
}
