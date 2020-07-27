#include "linked_list_test.h"
#include <time.h>
#include <assert.h>
#ifndef MY_DEBUG
#define MY_DEBUG
#endif

typedef int elem_type;
#define ERROR_ELEM 99999999
static elem_type *elem_calloc(size_type size)
{ /*size is the void*  size, usually 4*/
    elem_type *elem_p = (elem_type *)calloc(1, size);
    return elem_p;
}
static void elem_free(elem_type *elem_p)
{
    /* doing nothing is the best */
    return;
}

static elem_type elem_compare(const elem_type *elem_p_1, const elem_type *elem_p_2)
{
    assert(elem_p_1 != NULL);
    assert(elem_p_2 != NULL);

    return (*elem_p_1) - (*elem_p_2);
}

static void elem_print_nodecor(FILE *stream, const elem_type *elem_p)
{
    fprintf(stream, "%d", *(elem_p));
}
static void elem_deepcopy(elem_type *src, elem_type *dst)
{
    *dst = *src;
}
static ObjectFuncs elem_obj_funcs = {
    sizeof(elem_type *),
    sizeof(elem_type),
    elem_calloc,
    elem_free,
    elem_compare,
    elem_print_nodecor,
    elem_deepcopy};

elem_type parse_elem_p(elem_type *elem_p)
{
    if (elem_p != NULL)
    {
        return *elem_p;
    }
    else
    {
        return ERROR_ELEM;
    }
}
void linked_list_withhead_test_1(void)
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
        printf("%10d ", data_1[i]);
    }
    printf("\n");
    for (i = 0; i < 20; ++i)
    {
        printf("0x%p ", p_data_1[i]);
    }
    printf("\n");

    printf("data_2:\n");
    for (i = 0; i < 20; ++i)
    {
        printf("%10d ", data_2[i]);
    }
    printf("\n");
    for (i = 0; i < 20; ++i)
    {
        printf("0x%p ", p_data_2[i]);
    }
    printf("\n");

    printf("data_3:\n");
    for (i = 0; i < 20; ++i)
    {
        printf("%10d ", data_3[i]);
    }
    printf("\n");
    for (i = 0; i < 20; ++i)
    {
        printf("0x%p ", p_data_3[i]);
    }
    printf("\n");
    /**********************************/
    fprintf(stdout, "Test create: \n");
    int n;
    fprintf(stdout, "How much number do you want?(0-20)\n");
    scanf("%d", &n);
    LinkedList L1 = linked_list_new(n, &elem_obj_funcs);

    LinkedList L2 = linked_list_new_from_args(&elem_obj_funcs, 8,
                                              p_data_2[0], p_data_2[1], p_data_2[2], p_data_2[3],
                                              p_data_2[4], p_data_2[5], p_data_2[6], p_data_2[7]);

    LinkedList L3 = linked_list_new_from_objects(p_data_3, 12, &elem_obj_funcs);

    LinkedList L4 = linked_list_new_from_objects(NULL, 0, &elem_obj_funcs);
    fprintf(stdout, "Test create: OK\n");

    fprintf(stdout, "Test print: \n");
    linked_list_print(L1, "SINGLE_LINE");
    /* linked_list_print(L2, "MULTI_LINES"); */
    linked_list_print(L2, "SINGLE_LINE");
    linked_list_print(L3, "SINGLE_LINE");
    linked_list_print(L4, "SINGLE_LINE");
    fprintf(stdout, "Test print: OK\n");

    fprintf(stdout, "Test set:\n");
    fprintf(stdout, "Befor set, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    for (i = 0; i < n; ++i)
    {
        linked_list_index_assign(L1, i, p_data_1[i]);
    }
    fprintf(stdout, "after set, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
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
    fprintf(stdout, "L1 contains %d? %d\n", *p_data_1[3], linked_list_contains(L1, p_data_1[3]));
    fprintf(stdout, "L1 contains %d? %d\n", *p_data_1[18], linked_list_contains(L1, p_data_1[18]));
    fprintf(stdout, "L2 contains %d? %d\n", *p_data_2[4], linked_list_contains(L2, p_data_2[4]));
    fprintf(stdout, "L2 contains %d? %d\n", *p_data_2[19], linked_list_contains(L2, p_data_2[19]));
    fprintf(stdout, "L3 contains %d? %d\n", *p_data_3[13], linked_list_contains(L3, p_data_1[13]));
    fprintf(stdout, "L4 contains %d? %d\n", *p_data_1[0], linked_list_contains(L4, p_data_1[0]));
    fprintf(stdout, "Test contains: OK\n");

    fprintf(stdout, "Test append:\n");
    fprintf(stdout, "Before append, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    linked_list_append_object(L1, p_data_1[n]);
    fprintf(stdout, "After append, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "Before append, L4:\n");
    linked_list_print(L4, "SINGLE_LINE");
    linked_list_append_object(L4, p_data_1[n]);
    linked_list_append_object(L4, p_data_1[n + 1]);
    fprintf(stdout, "After append, L4\n");
    linked_list_print(L4, "SINGLE_LINE");
    fprintf(stdout, "Test append: OK\n");

    fprintf(stdout, "Test pop:\n");
    fprintf(stdout, "Before pop, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "Before pop, L4:\n");
    linked_list_print(L4, "SINGLE_LINE");
    elem_type *ret = linked_list_pop(L1);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    fprintf(stdout, "After pop, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "After pop, L4:\n");
    linked_list_print(L4, "SINGLE_LINE");
    fprintf(stdout, "try to pop L5:\n");
    LinkedList L5 = linked_list_new_from_objects(NULL, 0, &elem_obj_funcs);
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
    linked_list_print(L1, "SINGLE_LINE");
    linked_list_insert_object(L1, 0, p_data_2[0]);
    fprintf(stdout, "After insert %d to L1[0]:\n", *p_data_2[0]);
    linked_list_print(L1, "SINGLE_LINE");

    fprintf(stdout, "try to insert %d to L1[-1]:\n", *p_data_2[0]);
    linked_list_insert_object(L1, -1, p_data_2[0]);
    linked_list_print(L1, "SINGLE_LINE");

    fprintf(stdout, "try insert %d to L1[len(L1-1)]:\n", *p_data_2[0]);
    linked_list_insert_object(L1, linked_list_length(L1) - 1, p_data_2[0]);
    linked_list_print(L1, "SINGLE_LINE");

    fprintf(stdout, "try insert %d to L1[len(L1)]:\n", *p_data_2[0]);
    linked_list_insert_object(L1, linked_list_length(L1), p_data_2[0]);
    linked_list_print(L1, "SINGLE_LINE");

    fprintf(stdout, "try insert %d to L1[len(L1) +3]:\n", *p_data_2[0]);
    linked_list_insert_object(L1, linked_list_length(L1) + 3, p_data_2[0]);
    linked_list_print(L1, "SINGLE_LINE");

    fprintf(stdout, "try to insert %d to L5[0]:\n", *p_data_2[0]);
    linked_list_insert_object(L5, 0, p_data_2[0]);
    linked_list_print(L5, "SINGLE_LINE");
    fprintf(stdout, "Test insert: OK\n");

    fprintf(stdout, "Test pop_i:\n");
    fprintf(stdout, "Before pop_i, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "After pop_0, L1:\n");
    ret = linked_list_pop_i(L1, 0);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "After pop_2, L1:\n");
    ret = linked_list_pop_i(L1, 2);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "After pop_len(L1)-1, L1:\n");
    ret = linked_list_pop_i(L1, linked_list_length(L1) - 1);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "After pop_len(L1), L1:\n");
    ret = linked_list_pop_i(L1, linked_list_length(L1));
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    fprintf(stdout, "try to pop_0 L5:\n");
    ret = linked_list_pop_i(L5, 0);
    fprintf(stdout, "get popped elem: address: 0x%d, value: %d\n", ret, parse_elem_p(ret));
    fprintf(stdout, "Test pop_i: OK\n");

    fprintf(stdout, "Test reverse:\n");
    fprintf(stdout, "Before reverse, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    linked_list_reverse(L1);
    fprintf(stdout, "After reverse, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "Before reverse, L4:\n");
    linked_list_print(L4, "SINGLE_LINE");
    linked_list_reverse(L4);
    fprintf(stdout, "After reverse, L4:\n");
    linked_list_print(L4, "SINGLE_LINE");
    fprintf(stdout, "Before reverse, L5:\n");
    linked_list_print(L5, "SINGLE_LINE");
    linked_list_reverse(L5);
    fprintf(stdout, "After reverse, L5:\n");
    linked_list_print(L5, "SINGLE_LINE");
    fprintf(stdout, "Test reverse: OK\n");

    fprintf(stdout, "Test insert LinkedList:\n");
    fprintf(stdout, "Before insert, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "Before insert, L2:\n");
    linked_list_print(L2, "SINGLE_LINE");
    fprintf(stdout, "Now insert L2 to L1[2]\n");
    linked_list_insert_linked_list(L1, 2, L2);
    fprintf(stdout, "After insert, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    /* fprintf(stdout, "After insert, L2:\n");
    linked_list_print(L2, "SINGLE_LINE") */
    ;
    fprintf(stdout, "Before insert, L5:\n");
    linked_list_print(L5, "SINGLE_LINE");
    fprintf(stdout, "Now insert L5 to L1[2]\n");
    linked_list_insert_linked_list(L1, 2, L5);
    fprintf(stdout, "After insert, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "After insert, L5:\n");
    linked_list_print(L5, "SINGLE_LINE");
    fprintf(stdout, "Test insert LinkedList: OK\n");

    fprintf(stdout, "Test left search:\n");
    fprintf(stdout, "L3 is: ");
    linked_list_print(L3, "SINGLE_LINE");
    linked_list_append_object(L3, p_data_3[0]);
    linked_list_append_object(L3, p_data_3[1]);
    linked_list_append_object(L3, p_data_3[2]);
    linked_list_append_object(L3, p_data_3[3]);
    linked_list_append_object(L3, p_data_3[4]);
    fprintf(stdout, "After some append ,L3:\n");
    linked_list_print(L3, "SINGLE_LINE");
    int a3 = 200;
    fprintf(stdout, "left search %d in L3, result: %d\n", *p_data_3[2], linked_list_left_search(L3, p_data_3[2]));
    fprintf(stdout, "left search %d in L3, result: %d\n", a3, linked_list_left_search(L3, &a3));
    fprintf(stdout, "left search %d in L3, result: %d\n", *p_data_3[9], linked_list_left_search(L3, p_data_3[9]));
    fprintf(stdout, "Test left search: OK\n");
    fprintf(stdout, "Test right search:\n");
    fprintf(stdout, "right search %d in L3, result: %d\n", a3, linked_list_right_search(L3, &a3));
    fprintf(stdout, "right search %d in L3, result: %d\n", *p_data_3[9], linked_list_right_search(L3, p_data_3[9]));
    fprintf(stdout, "right search %d in L3, result: %d\n", *p_data_3[2], linked_list_right_search(L3, p_data_3[2]));
    fprintf(stdout, "Test rigth search: OK\n");

    fprintf(stdout, "Test left delete n same objects:\n");
    fprintf(stdout, "delete %d in L3 for %d times start from left:\n", *p_data_3[2], 999);
    linked_list_left_delete_objects(L3, p_data_3[2], 999);
    linked_list_print(L3, "SINGLE_LINE");
    fprintf(stdout, "delete %d in L3 for %d times start from left:\n", *p_data_3[3], 1);
    linked_list_left_delete_objects(L3, p_data_3[3], 1);
    linked_list_print(L3, "SINGLE_LINE");
    fprintf(stdout, "Test left delete n same objects: OK\n");
    fprintf(stdout, "Test right delete n same objects:\n");
    fprintf(stdout, "delete %d in L3 for %d times start from right:\n", *p_data_3[1], 999);
    linked_list_right_delete_objects(L3, p_data_3[1], 999);
    linked_list_print(L3, "SINGLE_LINE");
    fprintf(stdout, "delete %d in L3 for %d times start from right:\n", *p_data_3[4], 1);
    linked_list_right_delete_objects(L3, p_data_3[4], 1);
    linked_list_print(L3, "SINGLE_LINE");
    fprintf(stdout, "Test right delete n same objects: OK\n");

    fprintf(stdout, "Test slice:\nL1: ");
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "linked_list_slice(L1, 0, 199, 1): ");
    linked_list_print(linked_list_slice(L1, 0, 199, 1), "SINGLE_LINE");
    fprintf(stdout, "linked_list_slice(L1, 1, 10, 2): ");
    linked_list_print(linked_list_slice(L1, 1, 10, 2), "SINGLE_LINE");
    fprintf(stdout, "linked_list_slice(L1, 0, 12, 42): ");
    linked_list_print(linked_list_slice(L1, 0, 12, 42), "SINGLE_LINE");
    fprintf(stdout, "Test slice: OK\n");

    fprintf(stdout, "Test join:\n");
    fprintf(stdout, "Before join, L1:\n");
    linked_list_print(L1, "SINGLE_LINE");
    linked_list_join(L1, L4);
    fprintf(stdout, "After join L4 to L1's end:\n");
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "len of L1: %d\n", linked_list_length(L1));

    fprintf(stdout, "After join L5 to L1's end:\n");
    linked_list_join(L1, L5);
    linked_list_print(L1, "SINGLE_LINE");
    fprintf(stdout, "len of L1: %d\n", linked_list_length(L1));
    fprintf(stdout, "Test join: OK\n");

    fprintf(stdout, "Test free:\n");
    linked_list_free(L1);
    /* linked_list_free(L2); */
    linked_list_free(L3);
    /*   linked_list_free(L4); */
    linked_list_free(L5);
    fprintf(stdout, "Test free: OK\n");

    return;
}
/* 为什么free了L2. L2里面还有next和data域?然后L2->next, 自然是错误.*/