#include "vector_object_test.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#ifdef MY_DEBUG
#undef MY_DEBUG
#endif
typedef int elem_type;
static elem_type *elem_calloc(size_type size)
{ /*size is the void*  size, usually 4*/
    elem_type *elem_p = (elem_type *)calloc(1, size);
    return elem_p;
}
static void elem_free(elem_type **elem_p)
{
    if (*elem_p != NULL)
    {
        free(*elem_p);
        *elem_p = NULL;
    }
    return;
}

static elem_type elem_compare(const elem_type *elem_p_1, const elem_type *elem_p_2)
{
    assert(elem_p_1 != NULL);
    assert(elem_p_2 != NULL);

    return (*elem_p_1) - (*elem_p_2);
}

static void elem_print_noend(FILE *stream, const elem_type *elem_p)
{
    fprintf(stream, "%d\t", *(elem_p));
}
static void elem_deepcopy(void *dst, const void *src)
{
    *((elem_type *)dst) = *((elem_type *)src);
}
static ObjectFuncs elem_obj_funcs = {
    sizeof(elem_type *),
    sizeof(elem_type),
    elem_calloc,
    elem_free,
    elem_compare,
    elem_print_noend,
    elem_deepcopy};
typedef Vector Vector_Int;
void vector_object_test_2(void)
{
    fprintf(stdout, "Generate data:\n");
    elem_type *a1 = calloc(1, sizeof(elem_type));
    elem_type *a2 = calloc(1, sizeof(elem_type));
    elem_type *a3 = calloc(1, sizeof(elem_type));
    elem_type *a4 = calloc(1, sizeof(elem_type));
    elem_type *a5 = calloc(1, sizeof(elem_type));
    elem_type *a6 = calloc(1, sizeof(elem_type));
    elem_type *a7 = calloc(1, sizeof(elem_type));
    elem_type *a8 = calloc(1, sizeof(elem_type));
    elem_type *a9 = calloc(1, sizeof(elem_type));
    elem_type *a10 = calloc(1, sizeof(elem_type));

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
    fprintf(stdout, "Test vector_new:\n");
    fprintf(stdout, "Please input the size of vector (1-10)\n");
    elem_type n = 6;
    Vector_Int *vi1 = vector_new(n, &elem_obj_funcs);
    fprintf(stdout, "create vector: OK\n");
    fprintf(stdout, "Test vector_index_assign:\n");
    elem_type i;
    for (i = 0; i < n; ++i)
    {
        fprintf(stdout, "assigning %d...\n", i);
        vector_index_assign(vi1, i, data[i]);
    }
    fprintf(stdout, "assign complete\n");
    fprintf(stdout, "Test vector_length:\n");
    fprintf(stdout, "vi1 len: %d\n", vector_length(vi1));
    fprintf(stdout, "Test print:\n");
    vector_print(vi1, "SINGLE_LINE");
    fprintf(stdout, "print over.\n");

    fprintf(stdout, "Test vector_new_from_objects:\n");
    Vector_Int *vi2 = vector_new_from_objects((void **)(data + n), 7, &elem_obj_funcs);
    fprintf(stdout, "vi2:\n");
    vector_print(vi2, "SINGLE_LINE");

    fprintf(stdout, "Test vector_new_from_args:\n");
    Vector_Int *vi3 = vector_new_from_args(&elem_obj_funcs, 8, _a14, _a15, _a16, _a17, _a18, _a19, _a20, _a21);
    fprintf(stdout, "vi3:\n");
    vector_print(vi3, "SINGLE_LINE");

    fprintf(stdout, "Test vector_new_from_vector:\n");
    Vector_Int *vi4 = vector_new_from_vector(vi1);
    fprintf(stdout, "vi4:\n");
    vector_print(vi4, "SINGLE_LINE");

    fprintf(stdout, "Test vector_length:\n");
    fprintf(stdout, "vi2 vector_length: %d\n", vector_length(vi2));
    fprintf(stdout, "vi3 vector_length: %d\n", vector_length(vi3));
    fprintf(stdout, "vi4 vector_length: %d\n", vector_length(vi4));

    fprintf(stdout, "Test vector_object_at:\n");
    fprintf(stdout, "vi2[1] is ");
    elem_print_noend(stdout, vector_object_at(vi2, 1, FALSE));
    fprintf(stdout, "\n");

    fprintf(stdout, "vi3[-2] is ");
    elem_print_noend(stdout, vector_object_at(vi3, -2, TRUE));
    fprintf(stdout, "\n");

    fprintf(stdout, "Test vector_left_search:\n");

    elem_type a_temp_1[] = {3};
    elem_type a_temp_2[] = {8};
    fprintf(stdout, "try to find ");
    elem_print_noend(stdout, a_temp_1);
    fprintf(stdout, " in vi1:\n");
    fprintf(stdout, "%d\n", vector_left_search(vi1, a_temp_1));
    fprintf(stdout, "try to find ");
    elem_print_noend(stdout, a_temp_2);
    fprintf(stdout, " in vi2:\n");
    fprintf(stdout, "%d\n", vector_left_search(vi2, a_temp_2));

    fprintf(stdout, "Test vector_append:\nBefore append, vi2:\n");
    vector_print(vi2, "SINGLE_LINE");
    vector_append(vi2, a_temp_1);
    fprintf(stdout, "after append ");
    elem_print_noend(stdout, a_temp_1);
    fprintf(stdout, " to vi2, vi2 :\n ");
    vector_print(vi2, "SINGLE_LINE");

    fprintf(stdout, "Test vector_pop:\nBefore pop, vi4:\n");
    vector_print(vi4, "SINGLE_LINE");
    vector_pop(vi4);
    fprintf(stdout, "after pop, vi4:\n");
    vector_print(vi4, "SINGLE_LINE");

    fprintf(stdout, "Test vector_pop_i:\n");
    vector_pop_i(vi4, 2);
    fprintf(stdout, "after pop_2, vi4:\n");
    vector_print(vi4, "SINGLE_LINE");

    fprintf(stdout, "Test vector_insert:\n");
    vector_insert(vi4, 1, a_temp_2);
    fprintf(stdout, "after insert ");
    elem_print_noend(stdout, a_temp_2);
    fprintf(stdout, " to vi4 at location 1 :\n");
    vector_print(vi4, "SINGLE_LINE");

    fprintf(stdout, "Test vector_slice:\n");
    fprintf(stdout, "vi2, 0, 0, 1, is ");
    vector_print(vector_slice(vi2, 0, 0, 1, &elem_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vi2, -1, -1, 1, is ");
    vector_print(vector_slice(vi2, -1, -1, 1, &elem_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vi2, 1, 1, -1, is ");
    vector_print(vector_slice(vi2, 1, 1, -1, &elem_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vi2, -1, -1, -1, is ");
    vector_print(vector_slice(vi2, -1, -1, -1, &elem_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vi2, -4, -1, -2, is ");
    vector_print(vector_slice(vi2, -4, -1, -2, &elem_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vi2, -3, -1, 1, is ");
    vector_print(vector_slice(vi2, -3, -1, 1, &elem_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vi2, -1,-5, -1, is ");
    vector_print(vector_slice(vi2, -1, -5, -1, &elem_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vi2, 4,2, -2, is ");
    vector_print(vector_slice(vi2, 4, 2, -2, &elem_obj_funcs), "SINGLE_LINE");

    fprintf(stdout, "Test vector_reverse:\nBefore reverse, vi2:\n");
    vector_print(vi2, "SINGLE_LINE");
    vector_reverse(vi2, sizeof(elem_type *));
    vector_print(vi2, "SINGLE_LINE");

    fprintf(stdout, "Test vector_deep_copy:copy vi4 to vi2\nBefore copy, vi2:\n");
    vector_print(vi2, "SINGLE_LINE");
    vector_deep_copy(vi4, vi2);
    fprintf(stdout, "after copy, vi2:\n");
    vector_print(vi2, "SINGLE_LINE");

    fprintf(stdout, "Test vector_join:\nBefore join, vi3:\n");
    vector_print(vi3, "SINGLE_LINE");
    vector_join(vi3, vi4);
    fprintf(stdout, "after join(vi3, vi4), vi3:\n");
    vector_print(vi3, "SINGLE_LINE");

    fprintf(stdout, "Test free:\n");
    vector_free(vi1);
    vi1 = NULL;
    vector_free(vi2);
    vi2 = NULL;
    vector_free(vi3);
    vi3 = NULL;
    vector_free(vi4);
    vi4 = NULL;
    fprintf(stdout, "free over\n");

    fprintf(stdout, "Free original data:\n");
    free(a1);
    fprintf(stdout, "free over\n");
    free(a2);
    fprintf(stdout, "free over\n");
    free(a3);
    fprintf(stdout, "free over\n");
    free(a4);
    fprintf(stdout, "free over\n");
    free(a5);
    fprintf(stdout, "free over\n");
    free(a6);
    fprintf(stdout, "free over\n");
    free(a7);
    fprintf(stdout, "free over\n");
    free(a8);
    fprintf(stdout, "free over\n");
    free(a9);
    fprintf(stdout, "free over\n");
    free(a10);
    fprintf(stdout, "free over\n");
    free(a9);
    fprintf(stdout, "free over\n");
    free(a10);
    fprintf(stdout, "free over\n");
    fprintf(stdout, "Free original data: OK\n");
}
