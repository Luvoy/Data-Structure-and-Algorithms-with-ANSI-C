#include "vector_test.h"
#include <stdlib.h>
#include <stdio.h>
void vector_test(void)
{
    printf("Please input the size of vector:\n");
    int n = 0;
    scanf("%d", &n);
    Vector *v1 = vector_new((const size_type)(n));
    printf("create vector: OK\n");
    printf("len of v1 is %d\n", vector_length(v1));
    int i;
    for (i = 0; i < v1->used_size; ++i)
    {
        *((v1->items_p) + i) = 2 * i;
    }
    printf("v1 is : \n");
    vector_print(v1, "MULTI_LINE");
    printf("v1 itselt is: %p\n", v1);

    printf("test set v1:\n");
    vector_set(v1, 1, 99);
    printf("after set v1[1] to 99, v is: \n");
    vector_print(v1, "SINGLE_LINE");

    printf("try to set v1[-1]:\n");
    vector_set(v1, -1, 99);

    printf("try to set v1[20]:\n");
    vector_set(v1, 20, 99);

    vector_print(v1, "SINGLE_LINE");

    printf("test get:\n");
    printf("v[4] is %d\n", vector_get(v1, 4, TRUE));
    printf("v[-1] is %d\n", vector_get(v1, -1, FALSE));
    printf("v[-1] is %d\n", vector_get(v1, -1, TRUE));

    printf("test find:\n");
    printf("try to find 4 in v, result is %d\n", vector_find(v1, 4));
    printf("try to find 9879 in v, result is %d\n", vector_find(v1, 9879));

    printf("test copy:\n");
    Vector *v2 = vector_new_from_vector(v1);
    printf("v2 is:\n");
    vector_print(v2, "SINGLE_LINE");
    printf("address of v2:%p\n", &v2);
    printf("address of v1:%p\n", &v1);
    printf("v2 itself is %p\n", v2);
    printf("v1 itself is %p\n", v1);
    printf("v2 data area is %p\n", v2->items_p);
    printf("v1 data area is %p\n", v1->items_p);

    Vector *v3 = vector_new(100);
    printf("v3 is:\n");
    vector_print(v3, "SINGLE_LINE");
    printf("len of v3 is: %d\n", vector_len(v3));
    vector_copy(v1, v3);
    vector_print(v3, "SINGLE_LINE");
    printf("len of v3 is: %d\n", vector_len(v3));
    printf("address of v3:%p\n", &v3);
    printf("address of v1:%p\n", &v1);
    printf("v3 itself is %p\n", v3);
    printf("v1 itself is %p\n", v1);
    printf("v3 data area is %p\n", v3->items_p);
    printf("v1 data area is %p\n", v1->items_p);

    printf("test destroy:\n");
    printf("before destroy, v1 itselt is: %p\n", v1);
    vector_destroy(v1);
    printf("after destroy , v1 itself is %p\n", v1);
    v1 = NULL;

    printf("test append\n");
    vector_append(v2, 666);
    printf("after append , v2 is: ");
    vector_print(v2, "SINGLE_LINE");
    printf("len of v2 is: %d\n", vector_len(v2));

    printf("test pop\n");
    printf("pop result: %d\n", vector_pop(v3));
    printf("after pop , v3 is: ");
    vector_print(v3, "SINGLE_LINE");
    printf("len of v3 is: %d\n", vector_len(v3));

    printf("test pop i\n");
    printf("try to pop v2[3] , result: %d\n", vector_delete_i(v2, 3));
    printf("after pop , v2 is: ");
    vector_print(v2, "SINGLE_LINE");
    printf("len of v2 is: %d\n", vector_len(v2));

    printf("try to pop v3[999] , result: %d\n", vector_remove_i(v3, 999));
    printf("after pop , v3 is: ");
    vector_print(v3, "SINGLE_LINE");

    printf("test insert i\n");
    printf("try to insert %d at v2[3]\n", 888);
    vector_insert_i(v2, 3, 888);

    printf("after insert , v2 is: ");
    vector_print(v2, "SINGLE_LINE");
    printf("len of v2 is: %d\n", vector_len(v2));

    printf("try to insert %d at v3[999]\n", 888);
    vector_insert_i(v3, 999, 888);
    printf("after insert , v3 is: ");
    vector_print(v3, "SINGLE_LINE");

    printf("test join\n");
    printf("before join , v2 is: ");
    vector_print(v2, "SINGLE_LINE");
    printf("before join , v2 itselt is: %p\n", v2);

    printf("before join , v3 is: ");
    vector_print(v3, "SINGLE_LINE");
    printf("before join , v3 itselt is: %p\n", v3);
    vector_join(v2, v3);

    printf("after join , v2 is: ");
    vector_print(v2, "SINGLE_LINE");
    printf("after join , v3 itselt is: %p\n", v3);
    printf("after join , v3 's data area is: %p\n", v3->items_p);

    v3 = NULL;

    printf("test slice\n");
    printf("************  test 1 ************\n");
    Vector *v_slice_1 = vector_slice(v2, 0, 0, 1);
    printf("vector_slice(v2, 0, 0, 1) is: ");
    vector_print(v_slice_1, "SINGLE_LINE");

    printf("************  test 2 ************\n");
    Vector *v_slice_2 = vector_slice(v2, 0, 10, 2);
    printf("vector_slice(v2, 0, 10, 2) is: ");
    vector_print(v_slice_2, "SINGLE_LINE");

    printf("************  test 3 ************\n");
    Vector *v_slice_3 = vector_slice(v2, 0, 10, -1);
    printf("vector_slice(v2, 0, 10, -1) is: ");
    vector_print(v_slice_3, "SINGLE_LINE");

    printf("************  test 4 ************\n");
    Vector *v_slice_4 = vector_slice(v2, 8, 2, -3);
    printf("vector_slice(v2, 8, 2, -3) is: ");
    vector_print(v_slice_4, "SINGLE_LINE");

    printf("************  test 5 ************\n");
    Vector *v_slice_5 = vector_slice(v2, -1, -5, -1);
    printf("vector_slice(v2, -1, -5, -1) is: ");
    vector_print(v_slice_5, "SINGLE_LINE");

    printf("************  test 6 ************\n");
    Vector *v_slice_6 = vector_slice(v2, -7, -1, 2);
    printf("vector_slice(v2, -7, -1, 1) is: ");
    vector_print(v_slice_6, "SINGLE_LINE");

    printf("************  test 7 ************\n");
    Vector *v_slice_7 = vector_slice(v2, -1, -1, 1);
    printf("vector_slice(v2, -1, -1, 1) is: ");
    vector_print(v_slice_7, "SINGLE_LINE");

    printf("************  test 8 ************\n");
    Vector *v_slice_8 = vector_slice(v2, 1, 1, -1);
    printf("vector_slice(v2, 1, 1, -1) is: ");
    vector_print(v_slice_8, "SINGLE_LINE");

    printf("************  test 9 ************\n");
    Vector *v_slice_9 = vector_slice(v2, -1, -1, -1);
    printf("vector_slice(v2, -1, -1, -1) is: ");
    vector_print(v_slice_9, "SINGLE_LINE");

    printf("************  test 10 ************\n");
    Vector *v_slice_10 = vector_slice(v2, -7, -1, -2);
    printf("vector_slice(v2, -7, -1, -2) is: ");
    vector_print(v_slice_10, "SINGLE_LINE");

    printf("************  test 11 ************\n");
    Vector *v_slice_11 = vector_slice(v2, -2, -13, 2);
    printf("vector_slice(v2, -2, -13, 2) is: ");
    vector_print(v_slice_11, "SINGLE_LINE");

    printf("************  test 12 ************\n");
    Vector *v_slice_12 = vector_slice(v2, 14, 3, 4);
    printf("vector_slice(v2, 14, 3, 4) is: ");
    vector_print(v_slice_12, "SINGLE_LINE");

    printf("test reverse\nv2 befor reverse: ");
    vector_print(v2, "SINGLE_LINE");
    vector_reverse(v2);
    printf("v2 after reverse: ");
    vector_print(v2, "SINGLE_LINE");

    vector_destroy(v2);
    v2 = NULL;

    printf("test new from array\n");
    element_type a[] = {1, 2, 3, 4, 5};
    Vector *v4 = vector_new_from_elem_array(a, 5);
    vector_print(v4, "SINGLE_LINE");

    /* Vector *v5 = vector_new_from_elem_array({4, 4, 3, 1, 2}, 5);
    vector_print(v5, "SINGLE_LINE"); */
    /*错误!*/
    return;
}