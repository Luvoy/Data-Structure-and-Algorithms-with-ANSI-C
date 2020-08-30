#include "dlinked_list_headtail_test.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>

typedef int elem_type;
#define ERROR_ELEM 0xFFFFFFFF
static void elem_print(FILE *fp, const void *elem_p)
{
    if (elem_p == NULL)
    {
        fprintf(fp, "NULL");
    }
    else
    {
        fprintf(fp, "%d", *((const elem_type *)elem_p));
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
    int i;
    int data_1[20], data_2[20], data_3[20];
    for (i = 0; i < 20; ++i)
    {
        data_1[i] = rand() % 100; /*0-99*/
        data_2[i] = rand() % 100; /*0-99*/
        data_3[i] = rand() % 100; /*0-99*/
    }
    int *p_data_1[20], *p_data_2[20], *p_data_3[20];
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
        printf("%18d ", data_1[i]);
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
        printf("%18d ", data_2[i]);
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
        printf("%18d ", data_3[i]);
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
    int n;
    fprintf(stdout, "How much number do you want?(0-20)\n");
    /* scanf("%d", &n); */
    n = 6;
    DLinkedList L1 = linked_list_new(n);

    DLinkedList L2 = linked_list_new_from_args(8,
                                               p_data_2[0], p_data_2[1], p_data_2[2], p_data_2[3],
                                               p_data_2[4], p_data_2[5], p_data_2[6], p_data_2[7]);

    DLinkedList L3 = linked_list_new_from_elements(p_data_3, 12);

    DLinkedList L4 = linked_list_new_from_elements(NULL, 0);
    fprintf(stdout, "Test create: OK\n");

    fprintf(stdout, "Test print: \n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    /* linked_list_print(L2, "MULTI_LINES"); */
    linked_list_print(L2, "SINGLE_LINE", elem_print);
    linked_list_print(L3, "SINGLE_LINE", elem_print);
    linked_list_print(L4, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test print: OK\n");

    fprintf(stdout, "Test set:\n");
    fprintf(stdout, "Befor set, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    for (i = 0; i < n; ++i)
    {
        linked_list_index_assign(L1, i, p_data_1[i]);
    }
    fprintf(stdout, "after set, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "try to set L1[n]:(expect: out of index 0~n-1)\n");
    linked_list_index_assign(L1, n, p_data_1[n]);
    fprintf(stdout, "Test set: OK\n");

    fprintf(stdout, "Test length: \n");
    fprintf(stdout, "L1 length: %d\n", linked_list_length(L1));
    fprintf(stdout, "L2 length: %d\n", linked_list_length(L2));
    fprintf(stdout, "L3 length: %d\n", linked_list_length(L3));
    fprintf(stdout, "L4 length: %d\n", linked_list_length(L4));
    fprintf(stdout, "Test length: OK\n");

    fprintf(stdout, "Test contains: \n");
    fprintf(stdout, "L1 contains %d? %d\n", *p_data_1[3], linked_list_contains(L1, p_data_1[3], elem_compare));
    fprintf(stdout, "L1 contains %d? %d\n", *p_data_1[18], linked_list_contains(L1, p_data_1[18], elem_compare));
    fprintf(stdout, "L2 contains %d? %d\n", *p_data_2[4], linked_list_contains(L2, p_data_2[4], elem_compare));
    fprintf(stdout, "L2 contains %d? %d\n", *p_data_2[19], linked_list_contains(L2, p_data_2[19], elem_compare));
    fprintf(stdout, "L3 contains %d? %d\n", *p_data_3[13], linked_list_contains(L3, p_data_1[13], elem_compare));
    fprintf(stdout, "L4 contains %d? %d\n", *p_data_1[0], linked_list_contains(L4, p_data_1[0], elem_compare));
    fprintf(stdout, "Test contains: OK\n");

    fprintf(stdout, "Test append:\n");
    fprintf(stdout, "Before append, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    linked_list_append_object(L1, p_data_1[n]);
    fprintf(stdout, "After append, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Before append, L4:\n");
    linked_list_print(L4, "SINGLE_LINE", elem_print);
    linked_list_append_object(L4, p_data_1[n]);
    linked_list_append_object(L4, p_data_1[n + 1]);
    fprintf(stdout, "After append, L4\n");
    linked_list_print(L4, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test append: OK\n");

    fprintf(stdout, "Test pop:\n");
    fprintf(stdout, "Before pop, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Before pop, L4:\n");
    linked_list_print(L4, "SINGLE_LINE", elem_print);
    elem_type *ret = linked_list_pop(L1);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    fprintf(stdout, "After pop, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    ret = linked_list_pop(L4);
    fprintf(stdout, "After pop, L4:\n");
    linked_list_print(L4, "SINGLE_LINE", elem_print);
    fprintf(stdout, "try to pop L5:\n");
    DLinkedList L5 = linked_list_new_from_elements(NULL, 0);
    ret = linked_list_pop(L5);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    fprintf(stdout, "Test pop: OK\n");

    fprintf(stdout, "Test get:\n");
    fprintf(stdout, "L1[0]: address: 0x%p, value: %d\n", linked_list_object_at(L1, 0), parse_elem_p(linked_list_object_at(L1, 0)));
    fprintf(stdout, "L1[len(L1)-1]: address: 0x%p, value: %d\n", linked_list_object_at(L1, linked_list_length(L1) - 1), parse_elem_p(linked_list_object_at(L1, linked_list_length(L1) - 1)));
    fprintf(stdout, "L1[len(L1)]: address: 0x%p, value: %d\n", linked_list_object_at(L1, linked_list_length(L1)), parse_elem_p(linked_list_object_at(L1, linked_list_length(L1))));
    fprintf(stdout, "L5[0]: address: 0x%p, value: %d\n", linked_list_object_at(L5, 0), parse_elem_p(linked_list_object_at(L5, 0)));
    fprintf(stdout, "Test get: OK\n");

    fprintf(stdout, "Test insert:\n");
    fprintf(stdout, "Before insert, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    linked_list_insert_object(L1, 0, p_data_2[0]);
    fprintf(stdout, "After insert %d to L1[0]:\n", *p_data_2[0]);
    linked_list_print(L1, "SINGLE_LINE", elem_print);

    fprintf(stdout, "try to insert %d to L1[-1]:\n", *p_data_2[0]);
    linked_list_insert_object(L1, -1, p_data_2[0]);
    linked_list_print(L1, "SINGLE_LINE", elem_print);

    fprintf(stdout, "try insert %d to L1[len(L1-1)]:\n", *p_data_2[0]);
    linked_list_insert_object(L1, linked_list_length(L1) - 1, p_data_2[0]);
    linked_list_print(L1, "SINGLE_LINE", elem_print);

    fprintf(stdout, "try insert %d to L1[len(L1)]:\n", *p_data_2[0]);
    linked_list_insert_object(L1, linked_list_length(L1), p_data_2[0]);
    linked_list_print(L1, "SINGLE_LINE", elem_print);

    fprintf(stdout, "try insert %d to L1[len(L1) +3]:\n", *p_data_2[0]);
    linked_list_insert_object(L1, linked_list_length(L1) + 3, p_data_2[0]);
    linked_list_print(L1, "SINGLE_LINE", elem_print);

    fprintf(stdout, "try to insert %d to L5[0]:\n", *p_data_2[0]);
    linked_list_insert_object(L5, 0, p_data_2[0]);
    linked_list_print(L5, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test insert: OK\n");

    fprintf(stdout, "Test pop_i:\n");
    fprintf(stdout, "Before pop_i, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After pop_0, L1:\n");
    ret = linked_list_pop_i(L1, 0);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After pop_2, L1:\n");
    ret = linked_list_pop_i(L1, 2);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After pop_len(L1)-1, L1:\n");
    ret = linked_list_pop_i(L1, linked_list_length(L1) - 1);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After pop_len(L1), L1:\n");
    ret = linked_list_pop_i(L1, linked_list_length(L1));
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    fprintf(stdout, "try to pop_0 L5:\n");
    ret = linked_list_pop_i(L5, 0);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    fprintf(stdout, "Test pop_i: OK\n");

    fprintf(stdout, "Test reverse:\n");
    fprintf(stdout, "Before reverse, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    linked_list_reverse(L1);
    fprintf(stdout, "After reverse, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Before reverse, L4:\n");
    linked_list_print(L4, "SINGLE_LINE", elem_print);
    linked_list_reverse(L4);
    fprintf(stdout, "After reverse, L4:\n");
    linked_list_print(L4, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Before reverse, L5:\n");
    linked_list_print(L5, "SINGLE_LINE", elem_print);
    linked_list_reverse(L5);
    fprintf(stdout, "After reverse, L5:\n");
    linked_list_print(L5, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test reverse: OK\n");

    fprintf(stdout, "Test insert LinkedList:\n");
    fprintf(stdout, "Before insert, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Before insert, L2:\n");
    linked_list_print(L2, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Now insert L2 to L1[2]\n");
    linked_list_insert_linked_list(L1, 2, L2);
    fprintf(stdout, "After insert, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After insert, L2:\n");
    linked_list_print(L2, "SINGLE_LINE", elem_print);

    fprintf(stdout, "Before insert, L5:\n");
    linked_list_print(L5, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Now insert L5 to L1[2]\n");
    linked_list_insert_linked_list(L1, 2, L5);
    fprintf(stdout, "After insert, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "After insert, L5:\n");
    linked_list_print(L5, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test insert LinkedList: OK\n");

    fprintf(stdout, "Test left search:\n");
    fprintf(stdout, "L3 is: ");
    linked_list_print(L3, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Fistly append some number to L3:\n");
    linked_list_append_object(L3, p_data_3[0]);
    linked_list_append_object(L3, p_data_3[1]);
    linked_list_append_object(L3, p_data_3[2]);
    linked_list_append_object(L3, p_data_3[3]);
    linked_list_append_object(L3, p_data_3[4]);
    fprintf(stdout, "After some append ,L3:\n");
    linked_list_print(L3, "SINGLE_LINE", elem_print);
    int a3 = 200;
    fprintf(stdout, "left search %d in L3, result: %d\n", *p_data_3[2], linked_list_left_search(L3, p_data_3[2], elem_compare));
    fprintf(stdout, "left search %d in L3, result: %d\n", a3, linked_list_left_search(L3, &a3, elem_compare));
    fprintf(stdout, "left search %d in L3, result: %d\n", *p_data_3[9], linked_list_left_search(L3, p_data_3[9], elem_compare));
    fprintf(stdout, "Test left search: OK\n");
    fprintf(stdout, "Test right search:\n");
    fprintf(stdout, "right search %d in L3, result: %d\n", a3, linked_list_right_search(L3, &a3, elem_compare));
    fprintf(stdout, "right search %d in L3, result: %d\n", *p_data_3[9], linked_list_right_search(L3, p_data_3[9], elem_compare));
    fprintf(stdout, "right search %d in L3, result: %d\n", *p_data_3[2], linked_list_right_search(L3, p_data_3[2], elem_compare));
    fprintf(stdout, "Test rigth search: OK\n");

    fprintf(stdout, "Test left delete n same objects:\n");
    fprintf(stdout, "delete %d in L3 for %d times start from left:\n", *p_data_3[2], 999);
    linked_list_append_object(L3, p_data_3[2]);
    linked_list_print(L3, "SINGLE_LINE", elem_print);
    linked_list_left_delete_objects(L3, p_data_3[2], 999, elem_compare);
    linked_list_print(L3, "SINGLE_LINE", elem_print);
    fprintf(stdout, "delete %d in L3 for %d times start from left:\n", *p_data_3[3], 1);
    linked_list_left_delete_objects(L3, p_data_3[3], 1, elem_compare);
    linked_list_print(L3, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test left delete n same objects: OK\n");
    fprintf(stdout, "Test right delete n same objects:\n");
    fprintf(stdout, "delete %d in L3 for %d times start from right:\n", *p_data_3[1], 999);
    linked_list_append_object(L3, p_data_3[1]);
    linked_list_print(L3, "SINGLE_LINE", elem_print);
    linked_list_right_delete_objects(L3, p_data_3[1], 999, elem_compare);
    linked_list_print(L3, "SINGLE_LINE", elem_print);
    fprintf(stdout, "delete %d in L3 for %d times start from right:\n", *p_data_3[4], 1);
    linked_list_right_delete_objects(L3, p_data_3[4], 1, elem_compare);
    linked_list_print(L3, "SINGLE_LINE", elem_print);
    fprintf(stdout, "Test right delete n same objects: OK\n");

    fprintf(stdout, "Test slice:\nL1: ");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    DLinkedList temp_list;
    fprintf(stdout, "linked_list_slice(L1, 0, 199, 1): ");
    temp_list = linked_list_slice(L1, 0, 199, 1);
    linked_list_print(temp_list, "SINGLE_LINE", elem_print);
    if (temp_list != NULL)
    {
        linked_list_free(&temp_list);
    }
    fprintf(stdout, "linked_list_slice(L1, 1, 10, 2): ");
    temp_list = linked_list_slice(L1, 1, 10, 2);
    linked_list_print(temp_list, "SINGLE_LINE", elem_print);
    if (temp_list != NULL)
    {
        linked_list_free(&temp_list);
    }
    fprintf(stdout, "linked_list_slice(L1, 0, 12, 42): ");
    temp_list = linked_list_slice(L1, 0, 12, 42);
    linked_list_print(temp_list, "SINGLE_LINE", elem_print);
    if (temp_list != NULL)
    {
        linked_list_free(&temp_list);
    }
    fprintf(stdout, "Test slice: OK\n");

    fprintf(stdout, "Test join:\n");
    fprintf(stdout, "Before join, L1:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    linked_list_join(L1, L4);
    fprintf(stdout, "After join L4 to L1's end:\n");
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "len of L1: %d\n", linked_list_length(L1));

    fprintf(stdout, "After join L5 to L1's end:\n");
    linked_list_join(L1, L5);
    linked_list_print(L1, "SINGLE_LINE", elem_print);
    fprintf(stdout, "len of L1: %d\n", linked_list_length(L1));
    fprintf(stdout, "Test join: OK\n");

    fprintf(stdout, "Test free:\n");
    fprintf(stdout, "len of L1: %d\n", linked_list_length(L1));
    fprintf(stdout, "len of L2: %d\n", linked_list_length(L2));
    fprintf(stdout, "len of L3: %d\n", linked_list_length(L3));
    fprintf(stdout, "len of L4: %d\n", linked_list_length(L4));
    fprintf(stdout, "len of L5: %d\n", linked_list_length(L5));
    linked_list_free(&L1);
    linked_list_free(&L2);
    linked_list_free(&L3);
    linked_list_free(&L4);
    linked_list_free(&L5);
    fprintf(stdout, "Test free: OK\n");

    return;
}
/* 为什么free了L2. L2里面还有next和data域?然后L2->next, 自然是错误.*/