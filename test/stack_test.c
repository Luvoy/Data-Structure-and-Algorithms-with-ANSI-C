#include "stack_test.h"
#include "inttypes.h"
#include <stdlib.h>
void uint32p_print(FILE *fp, const void *elem_p)
{
    if (elem_p == NULL)
    {
        fprintf(fp, "NULL_ELEM");
    }
    else
    {
        fprintf(fp, "%" PRIu32 "", *(const uint32_t *)elem_p);
    }
}

extern void stack_test_1()
{
    //准备数据
    const uint32_t test_num = 20;
    uint32_t **a = (uint32_t **)malloc(test_num * sizeof(uint32_t *));
    assert(a != NULL);
    uint32_t i;
    for (i = 0; i < test_num; i++)
    {
        a[i] = (uint32_t *)malloc(sizeof(uint32_t));
        assert(a[i] != NULL);
        *a[i] = i;
    }

    //测试开始
    Stack *s1 = stack_new_empty(5);
    stack_print(s1, stdout, "MULTI_LINES", uint32p_print);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
    fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));

    fprintf(stdout, "\ntest clear s1\nafter clear, s1:\n");
    stack_clear(s1);
    stack_print(s1, stdout, "SINGLE_LINE", uint32p_print);

    fprintf(stdout, "\ntest get top from empty stack: \n");
    uint32p_print(stdout, stack_get_top(s1));
    fprintf(stdout, "\ntest get top from empty stack: OVER\n");

    fprintf(stdout, "\ntest pop from empty stack:\n");
    uint32p_print(stdout, stack_pop(s1));
    fprintf(stdout, "\ntest pop from empty stack: OVER\n");

    fprintf(stdout, "\ntest push:\nbefore push, s1:\n");
    stack_print(s1, stdout, "SINGLE_LINE", uint32p_print);
    stack_push(s1, a[0], TRUE);
    fprintf(stdout, "after push, s1:\n");
    stack_print(s1, stdout, "MULTI_LINES", uint32p_print);
    stack_print(s1, stdout, "SINGLE_LINE", uint32p_print);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
    fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));
    fprintf(stdout, "test push: OVER\n");

    fprintf(stdout, "\ntest get top from empty stack: \n");
    uint32p_print(stdout, stack_get_top(s1));
    fprintf(stdout, "\ntest get top from empty stack: OVER\n");

    fprintf(stdout, "\ntest pop:\nbefore pop, s1:\n");
    stack_print(s1, stdout, "SINGLE_LINE", uint32p_print);
    fprintf(stdout, "popped elem: ");
    uint32p_print(stdout, stack_pop(s1));
    fprintf(stdout, "\nafter pop, s1:\n");
    stack_print(s1, stdout, "MULTI_LINES", uint32p_print);
    stack_print(s1, stdout, "SINGLE_LINE", uint32p_print);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
    fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));

    const uint32_t push_num = 5;
    fprintf(stdout, "\ntest push for %" PRIu32 " times:\n", push_num);
    for (i = 1; i <= push_num; ++i)
    {
        fprintf(stdout, "---%" PRIu32 "---", i);
        stack_push(s1, a[i], TRUE);
        fprintf(stdout, "after push, s1:");
        stack_print(s1, stdout, "SINGLE_LINE", uint32p_print);
        fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
        fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
        fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
        fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));
    }
    fprintf(stdout, "test push for %" PRIu32 " times: OVER\n", push_num);

    fprintf(stdout, "\ntest push to a full stack: forbid auto expansion\n");
    stack_push(s1, a[6], FALSE);
    fprintf(stdout, "after push, s1:");
    stack_print(s1, stdout, "SINGLE_LINE", uint32p_print);
    fprintf(stdout, "\ntest push to a full stack: allow auto expansion\n");
    stack_push(s1, a[6], TRUE);
    fprintf(stdout, "after push, s1:");
    stack_print(s1, stdout, "SINGLE_LINE", uint32p_print);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
    fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));
    fprintf(stdout, "test push to a full stack: OVER\n");

    fprintf(stdout, "\ntest clear:\n");
    stack_clear(s1);
    fprintf(stdout, "after clear, s1:\n");
    stack_print(s1, stdout, "SINGLE_LINE", uint32p_print);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "test clear: OVER\n");

    fprintf(stdout, "\ntest free s1\n");
    stack_free(&s1);

    fprintf(stdout, "\ntest stack new from array:\n");
    Stack *s2 = stack_new_from_elements(10, 9, (void **)(a + 3));
    stack_print(s2, stdout, "SINGLE_LINE", uint32p_print);

    fprintf(stdout, "\ntest stack new from args:\n");
    Stack *s3 = stack_new_from_args(10, 6, a[19], a[18], a[17], a[16], a[15], a[14]);
    stack_print(s3, stdout, "MULTI_LINES", uint32p_print);

    fprintf(stdout, "\ntest free s2\n");
    stack_free(&s2);

    fprintf(stdout, "\ntest free s3\n");
    stack_free(&s3);

    //释放准备的数据
    for (i = 0; i < test_num; i++)
    {
        free(a[i]);
    }
    free(a);
}