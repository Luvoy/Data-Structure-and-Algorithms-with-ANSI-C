#include "vector_generic_test.h"
#include <stdio.h>
#include <assert.h>
typedef long elem_type;
typedef VectorGeneric VectorLong;
static void elem_print(FILE *fp, const void *elem)
{
    assert(elem);
    fprintf(fp, "%ld", *((const elem_type *)elem));
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
extern void vector_generic_test_1()
{
    fprintf(stdout, "Generate data:\n");

    elem_type a1 = 100000000L;
    elem_type a2 = 200000000L;
    elem_type a3 = 300000000L;
    elem_type a4 = 400000000L;
    elem_type a5 = 500000000L;
    elem_type a6 = 600000000L;
    elem_type a7 = 700000000L;
    elem_type a8 = 800000000L;
    elem_type a9 = 900000000L;
    elem_type a10 = 1000000000L;
    elem_type a11 = 1100000000L;
    elem_type a12 = 1200000000L;
    elem_type a13 = 1300000000L;
    elem_type a14 = 1400000000L;
    elem_type a15 = 1500000000L;
    elem_type a16 = 1600000000L;
    elem_type a17 = 1700000000L;
    elem_type a18 = 1800000000L;
    elem_type a19 = 1900000000L;
    elem_type a20 = 2000000000L;
    elem_type a21 = 2100000000L;
    elem_type data[] = {a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21};
    fprintf(stdout, "Generate data: OK\n");
    fprintf(stdout, "Test vector_generic_new:\n");
    fprintf(stdout, "Please input the size of vector (1-10)\n");
    vector_generic_index_type n = 6;
    VectorLong *vi1 = vector_generic_new(n, sizeof(elem_type));
    fprintf(stdout, "create vector: OK\n");
    fprintf(stdout, "Test vector_generic_indexassign:\n");
    elem_type i;
    for (i = 0; i < n; ++i)
    {
        fprintf(stdout, "assigning %d...\n", i);
        vector_generic_index_assign(vi1, i, &data[i]);
    }
    fprintf(stdout, "assign complete\n");
    fprintf(stdout, "Test vector_generic_length:\n");
    fprintf(stdout, "vi1 len: %d\n", vector_generic_length(vi1));
    fprintf(stdout, "Test print:\n");
    vector_generic_print(vi1, stdout, "SINGLE_LINE", elem_print);
    fprintf(stdout, "print over.\n");

    fprintf(stdout, "Test vector_generic_new_from_elements:\n");
    VectorLong *vi2 = vector_generic_new_from_elements(7, sizeof(elem_type), (void *)(data + n));
    fprintf(stdout, "vi2:\n");
    vector_generic_print(vi2, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_generic_new_fromargs:\n");
    VectorLong *vi3 = vector_generic_new_from_args(8, sizeof(elem_type), &a14, &a15, &a16, &a17, &a18, &a19, &a20, &a21);
    fprintf(stdout, "vi3:\n");
    vector_generic_print(vi3, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_generic_new_from_vector:\n");
    VectorLong *vi4 = vector_generic_new_from_vector(vi1);
    fprintf(stdout, "vi4:\n");
    vector_generic_print(vi4, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_generic_length:\n");
    fprintf(stdout, "vi2 vector_generic_length: %d\n", vector_generic_length(vi2));
    fprintf(stdout, "vi3 vector_generic_length: %d\n", vector_generic_length(vi3));
    fprintf(stdout, "vi4 vector_generic_length: %d\n", vector_generic_length(vi4));

    fprintf(stdout, "Test vector_generic_elementsat:\n");
    fprintf(stdout, "vi2[1] is ");
    elem_print(stdout, vector_generic_element_at(vi2, 1, FALSE));
    fprintf(stdout, "\n");

    fprintf(stdout, "vi3[-2] is ");
    elem_print(stdout, vector_generic_element_at(vi3, -2, TRUE));
    fprintf(stdout, "\n");

    fprintf(stdout, "Test vector_generic_left_search:\n");

    elem_type a_temp_1 = 300000000L;
    elem_type a_temp_2 = 800000000L;
    fprintf(stdout, "try to find ");
    elem_print(stdout, &a_temp_1);
    fprintf(stdout, " in vi1:\n");
    fprintf(stdout, "%d\n", vector_generic_left_search(vi1, &a_temp_1, elem_equal));
    fprintf(stdout, "try to find ");
    elem_print(stdout, &a_temp_2);
    fprintf(stdout, " in vi2:\n");
    fprintf(stdout, "%d\n", vector_generic_left_search(vi2, &a_temp_2, elem_equal));

    fprintf(stdout, "Test vector_genericappend:\nBefore append, vi2:\n");
    vector_generic_print(vi2, stdout, "SINGLE_LINE", elem_print);
    vector_generic_append(vi2, &a_temp_1);
    fprintf(stdout, "after append ");
    elem_print(stdout, &a_temp_1);
    fprintf(stdout, " to vi2, vi2 :\n ");
    vector_generic_print(vi2, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_generic_pop:\nBefore pop, vi4:\n");
    vector_generic_print(vi4, stdout, "SINGLE_LINE", elem_print);
    void *pop_temp = vector_generic_pop(vi4);
    fprintf(stdout, "after pop, vi4:\n");
    vector_generic_print(vi4, stdout, "SINGLE_LINE", elem_print);
    fprintf(stdout, "the poped elem is: ");
    elem_print(stdout, pop_temp);
    free(pop_temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif

    fprintf(stdout, "\nTest vector_generic_pop_i:\n");
    pop_temp = vector_generic_pop_i(vi4, 2);
    free(pop_temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    fprintf(stdout, "after pop_2, vi4:\n");
    vector_generic_print(vi4, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_generic_insert:\n");
    vector_generic_insert(vi4, 1, &a_temp_2);
    fprintf(stdout, "after insert ");
    elem_print(stdout, &a_temp_2);
    fprintf(stdout, " to vi4 at location 1 :\n");
    vector_generic_print(vi4, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_generic_slice:\n");
    VectorLong *vi_temp;
    fprintf(stdout, "vi2, 0, 0, 1, is ");
    vi_temp = vector_generic_slice(vi2, 0, 0, 1);
    vector_generic_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    vector_generic_free(&vi_temp);
    fprintf(stdout, "vi2, -1, -1, 1, is ");
    vi_temp = vector_generic_slice(vi2, -1, -1, 1);
    vector_generic_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    vector_generic_free(&vi_temp);
    fprintf(stdout, "vi2, 1, 1, -1, is ");
    vi_temp = vector_generic_slice(vi2, 1, 1, -1);
    vector_generic_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    vector_generic_free(&vi_temp);
    fprintf(stdout, "vi2, -1, -1, -1, is ");
    vi_temp = vector_generic_slice(vi2, -1, -1, -1);
    vector_generic_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    vector_generic_free(&vi_temp);
    fprintf(stdout, "vi2, -4, -1, -2, is ");
    vi_temp = vector_generic_slice(vi2, -4, -1, -2);
    vector_generic_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    vector_generic_free(&vi_temp);
    fprintf(stdout, "vi2, -3, -1, 1, is ");
    vi_temp = vector_generic_slice(vi2, -3, -1, 1);
    vector_generic_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    vector_generic_free(&vi_temp);
    fprintf(stdout, "vi2, -1,-5, -1, is ");
    vi_temp = vector_generic_slice(vi2, -1, -5, -1);
    vector_generic_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    vector_generic_free(&vi_temp);
    fprintf(stdout, "vi2, 4,2, -2, is ");
    vi_temp = vector_generic_slice(vi2, 4, 2, -2);
    vector_generic_print(vi_temp, stdout, "SINGLE_LINE", elem_print);
    vector_generic_free(&vi_temp);

    fprintf(stdout, "Test vector_generic_reverse:\nBefore reverse, vi2:\n");
    vector_generic_print(vi2, stdout, "SINGLE_LINE", elem_print);
    vector_generic_reverse(vi2);
    vector_generic_print(vi2, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test vector_generic_join:\nBefore join, vi3:\n");
    vector_generic_print(vi3, stdout, "SINGLE_LINE", elem_print);
    vector_generic_join(vi3, vi4);
    fprintf(stdout, "after join(vi3, vi4), vi3:\n");
    vector_generic_print(vi3, stdout, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Test free:\n");
    vector_generic_free(&vi1);
    vi1 = NULL;
    vector_generic_free(&vi2);
    vi2 = NULL;
    vector_generic_free(&vi3);
    vi3 = NULL;
    vector_generic_free(&vi4);
    vi4 = NULL;
    fprintf(stdout, "free over\n");
}
