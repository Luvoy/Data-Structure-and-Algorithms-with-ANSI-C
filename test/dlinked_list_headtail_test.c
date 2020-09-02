#include "dlinked_list_headtail_test.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

typedef int64_t elem_type;
#define ERROR_ELEM 0xFFFFFFFF
static void elem_print(FILE *fp, const void *elem_p)
{
    if (elem_p == NULL)
    {
        fprintf(fp, "NULL");
    }
    else
    {
        fprintf(fp, "%" PRId64 " ", *((const elem_type *)elem_p));
    }
}
static int elem_compare(const void *e1, const void *e2)
{
    assert(e1);
    assert(e2);
    return *((const elem_type *)e1) - *((const elem_type *)e2);
}
static elem_type parse_elem_p(void *p)
{
    if (p != NULL)
    {
        return *((elem_type *)p);
    }
    else
    {
        return ERROR_ELEM;
    }
}
void dlinked_list_headtail_test1(void)
{
    /*捏造数据*/
    srand((unsigned)time(NULL));
    int64_t i;
    int64_t data_1[20], data_2[20], data_3[20];
    for (i = 0; i < 20; ++i)
    {
        data_1[i] = rand() % 100; /*0-99*/
        data_2[i] = rand() % 100; /*0-99*/
        data_3[i] = rand() % 100; /*0-99*/
    }
    int64_t *p_data_1[20], *p_data_2[20], *p_data_3[20];
    for (i = 0; i < 20; ++i)
    {
        p_data_1[i] = &data_1[i];
        p_data_2[i] = &data_2[i];
        p_data_3[i] = &data_3[i];
    }
    /*测试一下捏造的数据*/
    printf("data_1:\n");
    for (i = 0; i < 20; ++i)
    {
        printf("%" PRId64 " ", data_1[i]);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    for (i = 0; i < 20; ++i)
    {
        printf("%18p ", p_data_1[i]);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");

    printf("data_2:\n");
    for (i = 0; i < 20; ++i)
    {
        printf("%" PRId64 " ", data_2[i]);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    for (i = 0; i < 20; ++i)
    {
        printf("%18p ", p_data_2[i]);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");

    printf("data_3:\n");
    for (i = 0; i < 20; ++i)
    {
        printf("% " PRId64 " ", data_3[i]);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    for (i = 0; i < 20; ++i)
    {
        printf("%18p ", p_data_3[i]);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    /**********************************/
    fprintf(stdout, "Test create: \n");
    int64_t n;
    fprintf(stdout, "How much number do you want?(0-20)\n");
    /* scanf("%"PRId64"", &n); */
    n = 6;
    DLinkedList L1 = dlinked_list_new(n);

    DLinkedList L2 = dlinked_list_new_from_args(8,
                                                p_data_2[0], p_data_2[1], p_data_2[2], p_data_2[3],
                                                p_data_2[4], p_data_2[5], p_data_2[6], p_data_2[7]);

    DLinkedList L3 = dlinked_list_new_from_elements((void **)p_data_3, 12);

    DLinkedList L4 = dlinked_list_new_from_elements(NULL, 0);
    fprintf(stdout, "Test create: OK\n");

    fprintf(stdout, "Test print: \n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    /* dlinked_list_print(L2, "MULTI_LINES"); */
    dlinked_list_print(L2, "SINGLE_LINE", elem_print);
    dlinked_list_print(L3, "SINGLE_LINE", elem_print);
    dlinked_list_print(L4, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test print: OK\n");

    fprintf(stdout, "Test set:\n");
    fprintf(stdout, "Befor set, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    for (i = 0; i < n; ++i)
    {
        dlinked_list_index_assign(L1, i, TRUE, p_data_1[i]);
    }
    fprintf(stdout, "after set, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "try to set L1[n]:(expect: out of index 0~n-1)\n");
    dlinked_list_index_assign(L1, n, TRUE, p_data_1[n]);
    fprintf(stdout, "Test set: OK\n");

    fprintf(stdout, "Test length: \n");
    fprintf(stdout, "L1 length: %" PRId64 "\n", dlinked_list_length(L1));
    fprintf(stdout, "L2 length: %" PRId64 "\n", dlinked_list_length(L2));
    fprintf(stdout, "L3 length: %" PRId64 "\n", dlinked_list_length(L3));
    fprintf(stdout, "L4 length: %" PRId64 "\n", dlinked_list_length(L4));
    fprintf(stdout, "Test length: OK\n");

    fprintf(stdout, "Test contains: \n");
    fprintf(stdout, "L1 contains %" PRId64 "? %" PRIu32 "\n", *p_data_1[3], dlinked_list_contains(L1, p_data_1[3], elem_compare));
    fprintf(stdout, "L1 contains %" PRId64 "? %" PRIu32 "\n", *p_data_1[18], dlinked_list_contains(L1, p_data_1[18], elem_compare));
    fprintf(stdout, "L2 contains %" PRId64 "? %" PRIu32 "\n", *p_data_2[4], dlinked_list_contains(L2, p_data_2[4], elem_compare));
    fprintf(stdout, "L2 contains %" PRId64 "? %" PRIu32 "\n", *p_data_2[19], dlinked_list_contains(L2, p_data_2[19], elem_compare));
    fprintf(stdout, "L3 contains %" PRId64 "? %" PRIu32 "\n", *p_data_3[13], dlinked_list_contains(L3, p_data_1[13], elem_compare));
    fprintf(stdout, "L4 contains %" PRId64 "? %" PRIu32 "\n", *p_data_1[0], dlinked_list_contains(L4, p_data_1[0], elem_compare));
    fprintf(stdout, "Test contains: OK\n");

    fprintf(stdout, "Test append:\n");
    fprintf(stdout, "Before append, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    dlinked_list_append_object(L1, p_data_1[n]);
    fprintf(stdout, "After append, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Before append, L4:\n");
    dlinked_list_print(L4, "SINGLE_LINE", elem_print);
    dlinked_list_append_object(L4, p_data_1[n]);
    dlinked_list_append_object(L4, p_data_1[n + 1]);
    fprintf(stdout, "After append, L4\n");
    dlinked_list_print(L4, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test append: OK\n");

    fprintf(stdout, "Test pop:\n");
    fprintf(stdout, "Before pop, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Before pop, L4:\n");
    dlinked_list_print(L4, "SINGLE_LINE", elem_print);
    elem_type *ret = dlinked_list_pop(L1);
    fprintf(stdout, "get popped elem: address: %p, value: %" PRId64 "\n", ret, parse_elem_p(ret));
    fprintf(stdout, "After pop, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    ret = dlinked_list_pop(L4);
    fprintf(stdout, "After pop, L4:\n");
    dlinked_list_print(L4, "SINGLE_LINE", elem_print);
    fprintf(stdout, "try to pop L5:\n");
    DLinkedList L5 = dlinked_list_new_from_elements(NULL, 0);
    ret = dlinked_list_pop(L5);
    fprintf(stdout, "get popped elem: address: 0x%p, value: %" PRId64 "\n", ret, parse_elem_p(ret));
    fprintf(stdout, "Test pop: OK\n");

    fprintf(stdout, "Test get:\n");
    fprintf(stdout, "L1[0]: address: %p, value: %" PRId64 "\n", dlinked_list_object_at(L1, 0, TRUE), parse_elem_p(dlinked_list_object_at(L1, 0, TRUE)));
    fprintf(stdout, "L1[len(L1)-1]: address: %p, value: %" PRId64 "\n", dlinked_list_object_at(L1, dlinked_list_length(L1) - 1, TRUE), parse_elem_p(dlinked_list_object_at(L1, dlinked_list_length(L1), TRUE)));
    fprintf(stdout, "L1[len(L1)]: address: %p, value: %" PRId64 "\n", dlinked_list_object_at(L1, dlinked_list_length(L1), TRUE), parse_elem_p(dlinked_list_object_at(L1, dlinked_list_length(L1), TRUE)));
    fprintf(stdout, "L5[0]: address: %p, value: %" PRId64 "\n", dlinked_list_object_at(L5, 0, TRUE), parse_elem_p(dlinked_list_object_at(L5, 0, TRUE)));
    fprintf(stdout, "Test get: OK\n");

    fprintf(stdout, "Test insert:\n");
    fprintf(stdout, "Before insert, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    dlinked_list_insert_object(L1, 0, p_data_2[0]);
    fprintf(stdout, "After insert %" PRId64 " to L1[0]:\n", *p_data_2[0]);
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);

    fprintf(stdout, "try to insert %" PRId64 " to L1[-1]:\n", *p_data_2[0]);
    dlinked_list_insert_object(L1, -1, p_data_2[0]);
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);

    fprintf(stdout, "try insert %" PRId64 " to L1[len(L1-1)]:\n", *p_data_2[0]);
    dlinked_list_insert_object(L1, dlinked_list_length(L1) - 1, p_data_2[0]);
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);

    fprintf(stdout, "try insert %" PRId64 " to L1[len(L1)]:\n", *p_data_2[0]);
    dlinked_list_insert_object(L1, dlinked_list_length(L1), p_data_2[0]);
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);

    fprintf(stdout, "try insert %" PRId64 " to L1[len(L1) +3]:\n", *p_data_2[0]);
    dlinked_list_insert_object(L1, dlinked_list_length(L1) + 3, p_data_2[0]);
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);

    fprintf(stdout, "try to insert %" PRId64 " to L5[0]:\n", *p_data_2[0]);
    dlinked_list_insert_object(L5, 0, p_data_2[0]);
    dlinked_list_print(L5, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test insert: OK\n");

    fprintf(stdout, "Test pop_i:\n");
    fprintf(stdout, "Before pop_i, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After pop_0, L1:\n");
    ret = dlinked_list_pop_i(L1, 0, TRUE);
    fprintf(stdout, "get popped elem: address: %p, value: %" PRId64 "\n", ret, parse_elem_p(ret));
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After pop_2, L1:\n");
    ret = dlinked_list_pop_i(L1, 2, TRUE);
    fprintf(stdout, "get popped elem: address: %p, value: %" PRId64 "\n", ret, parse_elem_p(ret));
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After pop_len(L1)-1, L1:\n");
    ret = dlinked_list_pop_i(L1, dlinked_list_length(L1) - 1, TRUE);
    fprintf(stdout, "get popped elem: address: %p, value: %" PRId64 "\n", ret, parse_elem_p(ret));
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After pop_len(L1), L1:\n");
    ret = dlinked_list_pop_i(L1, dlinked_list_length(L1), TRUE);
    fprintf(stdout, "get popped elem: address: %p, value: %" PRId64 "\n", ret, parse_elem_p(ret));
    fprintf(stdout, "try to pop_0 L5:\n");
    ret = dlinked_list_pop_i(L5, 0, TRUE);
    fprintf(stdout, "get popped elem: address: %p, value: %" PRId64 "\n", ret, parse_elem_p(ret));
    fprintf(stdout, "Test pop_i: OK\n");

    fprintf(stdout, "Test reverse:\n");
    fprintf(stdout, "Before reverse, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    dlinked_list_reverse(L1);
    fprintf(stdout, "After reverse, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Before reverse, L4:\n");
    dlinked_list_print(L4, "SINGLE_LINE", elem_print);
    dlinked_list_reverse(L4);
    fprintf(stdout, "After reverse, L4:\n");
    dlinked_list_print(L4, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Before reverse, L5:\n");
    dlinked_list_print(L5, "SINGLE_LINE", elem_print);
    dlinked_list_reverse(L5);
    fprintf(stdout, "After reverse, L5:\n");
    dlinked_list_print(L5, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test reverse: OK\n");

    fprintf(stdout, "Test insert LinkedList:\n");
    fprintf(stdout, "Before insert, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Before insert, L2:\n");
    dlinked_list_print(L2, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Now insert L2 to L1[2]\n");
    dlinked_list_insert_dlinked_list(L1, 2, &L2);
    fprintf(stdout, "After insert, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After insert, L2:\n");
    dlinked_list_print(L2, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Before insert, L5:\n");
    dlinked_list_print(L5, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Now insert L5 to L1[2]\n");
    dlinked_list_insert_dlinked_list(L1, 2, &L5);
    fprintf(stdout, "After insert, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After insert, L5:\n");
    dlinked_list_print(L5, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test insert LinkedList: OK\n");

    fprintf(stdout, "Test left search:\n");
    fprintf(stdout, "L3 is: ");
    dlinked_list_print(L3, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Fistly append some number to L3:\n");
    dlinked_list_append_object(L3, p_data_3[0]);
    dlinked_list_append_object(L3, p_data_3[1]);
    dlinked_list_append_object(L3, p_data_3[2]);
    dlinked_list_append_object(L3, p_data_3[3]);
    dlinked_list_append_object(L3, p_data_3[4]);
    fprintf(stdout, "After some append ,L3:\n");
    dlinked_list_print(L3, "SINGLE_LINE", elem_print);
    int64_t a3 = 200;
    fprintf(stdout, "left search %" PRId64 " in L3, result: %" PRId64 "\n", *p_data_3[2], dlinked_list_left_search(L3, p_data_3[2], elem_compare));
    fprintf(stdout, "left search %" PRId64 " in L3, result: %" PRId64 "\n", a3, dlinked_list_left_search(L3, &a3, elem_compare));
    fprintf(stdout, "left search %" PRId64 " in L3, result: %" PRId64 "\n", *p_data_3[9], dlinked_list_left_search(L3, p_data_3[9], elem_compare));
    fprintf(stdout, "Test left search: OK\n");
    fprintf(stdout, "Test right search:\n");
    fprintf(stdout, "right search %" PRId64 " in L3, result: %" PRId64 "\n", a3, dlinked_list_right_search(L3, &a3, elem_compare));
    fprintf(stdout, "right search %" PRId64 " in L3, result: %" PRId64 "\n", *p_data_3[9], dlinked_list_right_search(L3, p_data_3[9], elem_compare));
    fprintf(stdout, "right search %" PRId64 " in L3, result: %" PRId64 "\n", *p_data_3[2], dlinked_list_right_search(L3, p_data_3[2], elem_compare));
    fprintf(stdout, "Test rigth search: OK\n");

    fprintf(stdout, "Test left delete n same objects:\n");
    fprintf(stdout, "delete %" PRId64 " in L3 for %" PRId64 " times start from left:\n", *p_data_3[2], (uint64_t)999);
    dlinked_list_append_object(L3, p_data_3[2]);
    dlinked_list_print(L3, "SINGLE_LINE", elem_print);
    dlinked_list_left_delete_objects(L3, p_data_3[2], 999, elem_compare);
    dlinked_list_print(L3, "SINGLE_LINE", elem_print);
    fprintf(stdout, "delete %" PRId64 " in L3 for %" PRId64 " times start from left:\n", *p_data_3[3], (uint64_t)1);
    dlinked_list_left_delete_objects(L3, p_data_3[3], 1, elem_compare);
    dlinked_list_print(L3, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test left delete n same objects: OK\n");
    fprintf(stdout, "Test right delete n same objects:\n");
    fprintf(stdout, "delete %" PRId64 " in L3 for %" PRId64 " times start from right:\n", *p_data_3[1], (uint64_t)999);
    dlinked_list_append_object(L3, p_data_3[1]);
    dlinked_list_print(L3, "SINGLE_LINE", elem_print);
    dlinked_list_right_delete_objects(L3, p_data_3[1], 999, elem_compare);
    dlinked_list_print(L3, "SINGLE_LINE", elem_print);
    fprintf(stdout, "delete %" PRId64 " in L3 for %" PRId64 " times start from right:\n", *p_data_3[4], (uint64_t)1);
    dlinked_list_right_delete_objects(L3, p_data_3[4], 1, elem_compare);
    dlinked_list_print(L3, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test right delete n same objects: OK\n");

    fprintf(stdout, "Test slice:\nL1: ");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    DLinkedList temp_list;
    fprintf(stdout, "dlinked_list_slice(L1, 0, 199, 1): ");
    temp_list = dlinked_list_slice(L1, 0, 199, 1);
    dlinked_list_print(temp_list, "SINGLE_LINE", elem_print);
    if (temp_list != NULL)
    {
        dlinked_list_free(&temp_list);
    }
    fprintf(stdout, "dlinked_list_slice(L1, 1, 10, 2): ");
    temp_list = dlinked_list_slice(L1, 1, 10, 2);
    dlinked_list_print(temp_list, "SINGLE_LINE", elem_print);
    if (temp_list != NULL)
    {
        dlinked_list_free(&temp_list);
    }
    fprintf(stdout, "dlinked_list_slice(L1, 0, 12, 42): ");
    temp_list = dlinked_list_slice(L1, 0, 12, 42);
    dlinked_list_print(temp_list, "SINGLE_LINE", elem_print);
    if (temp_list != NULL)
    {
        dlinked_list_free(&temp_list);
    }
    fprintf(stdout, "Test slice: OK\n");

    fprintf(stdout, "Test join:\n");
    fprintf(stdout, "Before join, L1:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    dlinked_list_join(L1, L4);
    fprintf(stdout, "After join L4 to L1's end:\n");
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "len of L1: %" PRId64 "\n", dlinked_list_length(L1));

    fprintf(stdout, "After join L5 to L1's end:\n");
    dlinked_list_join(L1, L5);
    dlinked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "len of L1: %" PRId64 "\n", dlinked_list_length(L1));
    fprintf(stdout, "Test join: OK\n");

    fprintf(stdout, "Test free:\n");
    fprintf(stdout, "len of L1: %" PRId64 "\n", dlinked_list_length(L1));
    fprintf(stdout, "len of L2: %" PRId64 "\n", dlinked_list_length(L2));
    fprintf(stdout, "len of L3: %" PRId64 "\n", dlinked_list_length(L3));
    fprintf(stdout, "len of L4: %" PRId64 "\n", dlinked_list_length(L4));
    fprintf(stdout, "len of L5: %" PRId64 "\n", dlinked_list_length(L5));
    dlinked_list_free(&L1);
    dlinked_list_free(&L2);
    dlinked_list_free(&L3);
    dlinked_list_free(&L4);
    dlinked_list_free(&L5);
    fprintf(stdout, "Test free: OK\n");

    return;
}
/* 为什么free了L2. L2里面还有next和data域?然后L2->next, 自然是错误.*/