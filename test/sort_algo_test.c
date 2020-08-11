#include "sort_algo_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define elem_type long

int compare_1(const void *a, const void *b)
{
    return *(const elem_type *)a - *(const elem_type *)b;
}
void sort_test_1()
{
    fprintf(stdout, "test1:\n");
    /*  index:       0, 1, 2, 3, 4, 5, 6, 7, 8,  9, 10  */
    elem_type a[] = {4, 6, 5, 2, 8, 1, 9, 7, 0, -2, 3};
    /* bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    /* bubble_sort_improve(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    /* select_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    /* insert_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    /* shell_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    quick_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(elem_type); ++i)
    {
        fprintf(stdout, "%ld, ", a[i]);
    }
    fprintf(stdout, "\n");
}
void sort_test_11()
{
    fprintf(stdout, "test11: 2 elements test\n");
    elem_type a[] = {6, 5};
    /* bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    /* bubble_sort_improve(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    /* select_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    /* insert_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    /* shell_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    quick_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(elem_type); ++i)
    {
        fprintf(stdout, "%ld, ", a[i]);
    }
    fprintf(stdout, "\n");
}
int compare_1_descent(const void *a, const void *b)
{
    return *(const elem_type *)b - *(const elem_type *)a;
}
void sort_test_111()
{
    fprintf(stdout, "test111: descent test\n");
    elem_type a[] = {4, 6, 5, 2, 8, 1, 9, 7, 0, -2, 3};
    /* bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1); */
    /* bubble_sort_improve(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1_descent); */
    /* select_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1_descent); */
    /* insert_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1_descent); */
    /* shell_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1_descent); */
    quick_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_1_descent);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(elem_type); ++i)
    {
        fprintf(stdout, "%ld, ", a[i]);
    }
    fprintf(stdout, "\n");
}

/* --------------------test2------------------------*/
typedef struct Student
{
    const char *name;
    uint8_t age;
} Student;

#define elem_type Student

int compare_2(const void *a, const void *b)
{
    /*  return ((const elem_type *)a)->age - ((const elem_type *)b)->age; */
    return (*((const elem_type *)a)).age - (*((const elem_type *)b)).age;
}
void sort_test_2()
{
    fprintf(stdout, "test2:\n");
    elem_type stu1 = {"Tom", 4};
    elem_type stu2 = {"Amy", 6};
    elem_type stu3 = {"Miguel", 5};
    elem_type stu4 = {"Jacob", 2};
    elem_type stu5 = {"Alex", 8};
    elem_type stu6 = {"Bob", 1};
    elem_type stu7 = {"John", 7};
    elem_type stu8 = {"Tony", 3};

    elem_type a[] = {stu1, stu2, stu3, stu4, stu5, stu6, stu7, stu8};
    /* bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_2); */
    /* bubble_sort_improve(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_2); */
    /* select_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_2); */
    /* insert_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_2); */
    /* shell_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_2); */
    quick_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_2);

    int i;
    for (i = 0; i < sizeof(a) / sizeof(elem_type); ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i].name, a[i].age);
    }
    fprintf(stdout, "\n");
}
void sort_test_22()
{
    fprintf(stdout, "test22:\n");
    elem_type stu1 = {"Tom", 4};
    elem_type stu2 = {"Amy", 6};
    elem_type stu3 = {"Miguel", 5};
    elem_type stu4 = {"Jacob", 2};
    elem_type stu5 = {"Alex", 8};
    elem_type stu6 = {"Bob", 1};
    elem_type stu7 = {"John", 7};
    elem_type stu8 = {"Tony", 3};

    elem_type *a = (elem_type *)malloc(8 * sizeof(elem_type));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    *(a + 0) = stu1;
    *(a + 1) = stu2;
    *(a + 2) = stu3;
    *(a + 3) = stu4;
    *(a + 4) = stu5;
    *(a + 5) = stu6;
    *(a + 6) = stu7;
    *(a + 7) = stu8;
    fprintf(stdout, "before sort:\n");
    int i;
    for (i = 0; i < 8; ++i)
    {

        fprintf(stdout, "%s: %d\n", a[i].name, a[i].age);
    }
    fprintf(stdout, "after sort:\n");
    /* bubble_sort(a, 8, (elem_size_type)sizeof(elem_type), compare_2); */
    /* bubble_sort_improve(a, 8, (elem_size_type)sizeof(elem_type), compare_2); */
    /* select_sort(a, 8, (elem_size_type)sizeof(elem_type), compare_2); */
    /* insert_sort(a, 8, (elem_size_type)sizeof(elem_type), compare_2); */
    /* shell_sort(a, 8, (elem_size_type)sizeof(elem_type), compare_2); */
    quick_sort(a, 8, (elem_size_type)sizeof(elem_type), compare_2);
    for (i = 0; i < 8; ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i].name, a[i].age);
    }
    free(a);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
}
/* --------------------test3------------------------*/

#define elem_type Student *

int compare_3(const void *a, const void *b)
{
    /* return *(const elem_type *)a - *(const elem_type *)b; */ /*错!*/
    return ((*((elem_type *)a))->age) - ((*((elem_type *)b))->age);

    /* return (uint8_t)(((*((elem_type *)a))->age) - ((*((elem_type *)b))->age)); */ /*错!*/
    /* return (uint8_t)(((elem_type)a)->age - ((elem_type)b)->age); */               /*错!*/
}
void sort_test_3()
{
    fprintf(stdout, "test3:\n");
    Student stu1 = {"Tom", 4};
    Student stu2 = {"Amy", 6};
    Student stu3 = {"Miguel", 5};
    Student stu4 = {"Jacob", 2};
    /* Student stu5 = {"Alex", 8};
    Student stu6 = {"Bob", 1};
    Student stu7 = {"John", 7};
    Student stu8 = {"Tony", 3}; */
    elem_type stu5_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu5_->name = "Alex";
    stu5_->age = 8;
    elem_type stu6_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu6_->name = "Bob";
    stu6_->age = 1;
    elem_type stu7_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu7_->name = "John";
    stu7_->age = 7;
    elem_type stu8_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu8_->name = "Tony";
    stu8_->age = 3;

    elem_type a[] = {&stu1, &stu2, &stu3, &stu4, stu5_, stu6_, stu7_, stu8_};
    printf("a:0x%p, a+1:0x%p, a+2:0x%p\n", a, a + 1, a + 2);
    printf("*a:0x%p, *(a+1):0x%p, *(a+2):0x%p\n", *a, *(a + 1), *(a + 2));
    printf("*a->age:%d, *(a+1)->age:%d, *(a+2)->age:%d\n", (*a)->age, (*(a + 1))->age, (*(a + 2))->age);
    /* bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    /* bubble_sort_improve(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    /* select_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    /* insert_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    /* shell_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    quick_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(elem_type); ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i]->name, a[i]->age);
    }
    fprintf(stdout, "********************************\n");
    free(stu5_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(stu6_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(stu7_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(stu8_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
}
void sort_test_33()
{
    fprintf(stdout, "test33:\n");
    Student stu1 = {"Tom", 4};
    Student stu2 = {"Amy", 6};
    Student stu3 = {"Miguel", 5};
    Student stu4 = {"Jacob", 2};
    Student stu5 = {"Alex", 8};
    Student stu6 = {"Bob", 1};
    Student stu7 = {"John", 7};
    Student stu8 = {"Tony", 3};

    elem_type a[] = {&stu1, &stu2, &stu3, &stu4, &stu5, &stu6, &stu7, &stu8};
    /* bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3);*/
    /* bubble_sort_improve(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    /* select_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    /* insert_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    /* shell_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    quick_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(long); ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i]->name, a[i]->age);
    }
    fprintf(stdout, "********************************\n");
}

void sort_test_333()
{
    fprintf(stdout, "test333:\n");
    elem_type stu1_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu1_->name = "Tom";
    stu1_->age = 4;
    elem_type stu2_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu2_->name = "Amy";
    stu2_->age = 6;
    elem_type stu3_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu3_->name = "Miguel";
    stu3_->age = 5;
    elem_type stu4_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu4_->name = "Jacob";
    stu4_->age = 2;
    elem_type stu5_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu5_->name = "Alex";
    stu5_->age = 8;
    elem_type stu6_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu6_->name = "Bob";
    stu6_->age = 1;
    elem_type stu7_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu7_->name = "John";
    stu7_->age = 7;
    elem_type stu8_ = (elem_type)malloc(sizeof(Student));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    stu8_->name = "Tony";
    stu8_->age = 3;

    elem_type a[] = {stu1_, stu2_, stu3_, stu4_, stu5_, stu6_, stu7_, stu8_};
    /* bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3);*/
    /* bubble_sort_improve(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    /* select_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    /* insert_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    /* shell_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3); */
    quick_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_3);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(elem_type); ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i]->name, a[i]->age);
    }
    fprintf(stdout, "********************************\n");
    free(stu1_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(stu2_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(stu3_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(stu4_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(stu5_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(stu6_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(stu7_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(stu8_);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
}
int compare_4(const void *a, const void *b)
{
    return strcmp(((*((elem_type *)a))->name), ((*((elem_type *)b))->name));
}
void sort_test_4()
{
    fprintf(stdout, "test4:sort by name\n");
    Student stu1 = {"Tom", 4};
    Student stu2 = {"Amy", 6};
    Student stu3 = {"Miguel", 5};
    Student stu4 = {"Jacob", 2};
    Student stu5 = {"Alex", 8};
    Student stu6 = {"Bob", 1};
    Student stu7 = {"John", 7};
    Student stu8 = {"Tony", 3};

    elem_type a[] = {&stu1, &stu2, &stu3, &stu4, &stu5, &stu6, &stu7, &stu8};
    /* bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_4);*/
    /* bubble_sort_improve(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_4); */
    /* select_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_4); */
    /* insert_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_4); */
    /* shell_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_4); */
    quick_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), (elem_size_type)sizeof(elem_type), compare_4);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(long); ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i]->name, a[i]->age);
    }
    fprintf(stdout, "********************************\n");
}
