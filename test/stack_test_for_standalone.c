#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// typedef int64_t cmp_ret_type;
// #define STACK_T int32_t
// #define STACK_T_NULL ((STACK_T)0)

// void elem_print_imp(FILE *fp, const STACK_T e, cmp_ret_type (*cmp)(const STACK_T, const STACK_T))
// { //implement: void(*)(FILE*,const STACK_T, cmp_ret_type(*)(const STACK_T, const STACK_T))
//     if (cmp(e, STACK_T_NULL) == 0)
//     {
//         fprintf(fp, "NULL_ELEM");
//     }
//     else
//     {
//         fprintf(fp, "%" PRId32 "", e);
//     }
// }

// cmp_ret_type elem_cmp_imp(const STACK_T a, const STACK_T b)
// { //implement: cmp_ret_type(*)(const STACK_T, const STACK_T)
//     return a - b;
// }

// #include "../Stack/stack_standalone.h"

// #define STACK_T int32_t
// #define STACK_T_NULL ((STACK_T)0)
typedef int64_t cmp_ret_type;
typedef struct Student
{
    char *name;
    uint32_t age;
} Student;
#define STACK_T Student
#define STACK_T_NULL ((Student){NULL, 0})

void elem_print_imp(FILE *fp, const STACK_T e, cmp_ret_type (*cmp)(const STACK_T, const STACK_T))
{ //implement: void(*)(FILE*,const STACK_T, cmp_ret_type(*)(const STACK_T, const STACK_T))
    if (cmp(e, STACK_T_NULL) == 0)
    {
        fprintf(fp, "NULL_ELEM");
    }
    else
    {
        fprintf(fp, "%s: %" PRIu32 "", e.name, e.age);
    }
}
#include <string.h>

cmp_ret_type elem_cmp_imp(const STACK_T a, const STACK_T b)
{ //implement: cmp_ret_type(*)(const STACK_T, const STACK_T)
    if (a.name == NULL && a.age == 0 && b.name == NULL && b.age == 0)
    {
        return 0;
    }
    if (a.name != NULL && b.name != NULL)
    {
        return a.age - b.age + strcmp(a.name, b.name);
    }
    else
    {
        return a.age - b.age + 1;
    }
}

#include "../Stack/stack_standalone.h"

#define STACK_T Student
#define STACK_T_NULL ((STACK_T){NULL, 0})

void stack_test_for_standalone_1()
{
    //准备数据
    const uint32_t test_num = 20;
    STACK_T *a = (STACK_T *)malloc(test_num * sizeof(STACK_T));
    assert(a != NULL);
    uint32_t i;
    for (i = 0; i < test_num; i++)
    {
        a[i] = (STACK_T){"Tom", i};
        // a[i] = (STACK_T)i;
    }
    //测试开始
    Stack *s1 = stack_new_empty(5);
    stack_print(s1, stdout, "MULTI_LINES", elem_print_imp, elem_cmp_imp);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
    fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));

    fprintf(stdout, "\ntest clear s1\nafter clear, s1:\n");
    stack_clear(s1);
    stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);

    fprintf(stdout, "\ntest get top from empty stack: \n");
    elem_print_imp(stdout, stack_get_top(s1), elem_cmp_imp);
    fprintf(stdout, "\ntest get top from empty stack: OVER\n");

    fprintf(stdout, "\ntest pop from empty stack:\n");
    elem_print_imp(stdout, stack_pop(s1), elem_cmp_imp);
    fprintf(stdout, "\ntest pop from empty stack: OVER\n");

    fprintf(stdout, "\ntest push:\nbefore push, s1:\n");
    stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);
    stack_push(s1, a[0], TRUE, elem_cmp_imp);
    fprintf(stdout, "after push, s1:\n");
    stack_print(s1, stdout, "MULTI_LINES", elem_print_imp, elem_cmp_imp);
    stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
    fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));
    fprintf(stdout, "test push: OVER\n");

    fprintf(stdout, "\ntest push NULL_ELEM:\nbefore push, s1:\n");
    stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);
    stack_push(s1, STACK_T_NULL, TRUE, elem_cmp_imp);
    fprintf(stdout, "after push, s1:\n");
    stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
    fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));
    fprintf(stdout, "\ntest push NULL_ELEM: OVER\n");

    fprintf(stdout, "\ntest get top from empty stack: \n");
    elem_print_imp(stdout, stack_get_top(s1), elem_cmp_imp);
    fprintf(stdout, "\ntest get top from empty stack: OVER\n");

    fprintf(stdout, "\ntest pop:\nbefore pop, s1:\n");
    stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);
    fprintf(stdout, "popped elem: ");
    elem_print_imp(stdout, stack_pop(s1), elem_cmp_imp);
    fprintf(stdout, "\nafter pop, s1:\n");
    stack_print(s1, stdout, "MULTI_LINES", elem_print_imp, elem_cmp_imp);
    stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
    fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));

    const uint32_t push_num = 5;
    fprintf(stdout, "\ntest push for %" PRIu32 " times:\n", push_num);
    for (i = 1; i <= push_num; ++i)
    {
        fprintf(stdout, "---%" PRIu32 "---", i);
        stack_push(s1, a[i], TRUE, elem_cmp_imp);
        fprintf(stdout, "after push, s1:");
        stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);
        fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
        fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
        fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
        fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));
    }
    fprintf(stdout, "test push for %" PRIu32 " times: OVER\n", push_num);

    fprintf(stdout, "\ntest push to a full stack: forbid auto expansion\n");
    stack_push(s1, a[6], FALSE, elem_cmp_imp);
    fprintf(stdout, "after push, s1:");
    stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);
    fprintf(stdout, "\ntest push to a full stack: allow auto expansion\n");
    stack_push(s1, a[6], TRUE, elem_cmp_imp);
    fprintf(stdout, "after push, s1:");
    stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "s1 total size: %" PRId64 "\n", stack_total_size(s1));
    fprintf(stdout, "s1 used size: %" PRId64 "\n", stack_used_size(s1));
    fprintf(stdout, "test push to a full stack: OVER\n");

    fprintf(stdout, "\ntest clear:\n");
    stack_clear(s1);
    fprintf(stdout, "after clear, s1:\n");
    stack_print(s1, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);
    fprintf(stdout, "s1 is empty?: %" PRI_MY_BOOL "\n", stack_is_empty(s1));
    fprintf(stdout, "s1 is full?: %" PRI_MY_BOOL "\n", stack_is_full(s1));
    fprintf(stdout, "test clear: OVER\n");

    fprintf(stdout, "\ntest free s1\n");
    stack_free(&s1);

    fprintf(stdout, "\ntest stack new from array:\n");
    Stack *s2 = stack_new_from_elements(10, 9, (STACK_T *)(a + 3));
    stack_print(s2, stdout, "SINGLE_LINE", elem_print_imp, elem_cmp_imp);

    fprintf(stdout, "\ntest stack new from args:\n");
    Stack *s3 = stack_new_from_args(10, 6, a[19], a[18], a[17], a[16], a[15], a[14]);
    stack_print(s3, stdout, "MULTI_LINES", elem_print_imp, elem_cmp_imp);

    fprintf(stdout, "\ntest free s2\n");
    stack_free(&s2);

    fprintf(stdout, "\ntest free s3\n");
    stack_free(&s3);

    //释放准备的数据
    free(a);
}

int main(int argc, char const *argv[])
{
    printf("argc: %d\n", argc);
    printf("agrv: %s\n", *argv);
    stack_test_for_standalone_1();
    return 0;
}
