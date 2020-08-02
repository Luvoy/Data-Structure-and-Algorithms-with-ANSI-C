#include "sort_algo_test.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef ELEMENT_TYPE
#define elem_type long
#endif
int compare_1(const void *a, const void *b)
{
    return *(const elem_type *)a - *(const elem_type *)b;
}
void sort_test_1()
{
    elem_type a[] = {4, 6, 5, 2, 8, 1, 9, 7, 0, -2, 3};
    bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), compare_1);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(long); ++i)
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
#ifdef ELEMENT_TYPE
#define elem_type Student
#endif
int compare_2(const void *a, const void *b)
{
    printf("left: %d, right: %d ", ((const elem_type *)a)->age, ((const elem_type *)b)->age);
    return ((const elem_type *)a)->age - ((const elem_type *)b)->age;
}
void sort_test_2()
{ /*测试失败*/
    elem_type stu1 = {"Tom", 4};
    elem_type stu2 = {"Amy", 6};
    elem_type stu3 = {"Miguel", 5};
    elem_type stu4 = {"Jacob", 2};
    elem_type stu5 = {"Alex", 8};
    elem_type stu6 = {"Bob", 1};
    elem_type stu7 = {"John", 7};
    elem_type stu8 = {"Tony", 3};

    elem_type a[] = {stu1, stu2, stu3, stu4, stu5, stu6, stu7, stu8};
    bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), compare_2);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(elem_type); ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i].name, a[i].age);
    }
    fprintf(stdout, "\n");
}
void sort_test_22()
{ /*测试失败*/
    elem_type stu1 = {"Tom", 4};
    elem_type stu2 = {"Amy", 6};
    elem_type stu3 = {"Miguel", 5};
    elem_type stu4 = {"Jacob", 2};
    elem_type stu5 = {"Alex", 8};
    elem_type stu6 = {"Bob", 1};
    elem_type stu7 = {"John", 7};
    elem_type stu8 = {"Tony", 3};

    elem_type *a = (elem_type *)malloc(8 * sizeof(elem_type));
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
    bubble_sort(a, 8, compare_2);
    for (i = 0; i < 8; ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i].name, a[i].age);
    }
    free(a);
}
/* --------------------test3------------------------*/
#ifdef ELEMENT_TYPE
#define elem_type Student *
#endif
int compare_3(const void *a, const void *b)
{
    /* return *(const elem_type *)a - *(const elem_type *)b; */ /*错!*/
    return ((*((elem_type *)a))->age) - ((*((elem_type *)b))->age);

    /* return (uint8_t)(((*((elem_type *)a))->age) - ((*((elem_type *)b))->age)); */ /*错!*/
    /* return (uint8_t)(((elem_type)a)->age - ((elem_type)b)->age); */               /*错!*/
}
void sort_test_3()
{
    Student stu1 = {"Tom", 4};
    Student stu2 = {"Amy", 6};
    Student stu3 = {"Miguel", 5};
    Student stu4 = {"Jacob", 2};
    /* Student stu5 = {"Alex", 8};
    Student stu6 = {"Bob", 1};
    Student stu7 = {"John", 7};
    Student stu8 = {"Tony", 3}; */
    elem_type stu5_ = (elem_type)malloc(sizeof(Student));
    stu5_->name = "Alex";
    stu5_->age = 8;
    elem_type stu6_ = (elem_type)malloc(sizeof(Student));
    stu6_->name = "Bob";
    stu6_->age = 1;
    elem_type stu7_ = (elem_type)malloc(sizeof(Student));
    stu7_->name = "John";
    stu7_->age = 7;
    elem_type stu8_ = (elem_type)malloc(sizeof(Student));
    stu8_->name = "Tony";
    stu8_->age = 3;

    elem_type a[] = {&stu1, &stu2, &stu3, &stu4, stu5_, stu6_, stu7_, stu8_};
    printf("a:0x%p, a+1:0x%p, a+2:0x%p\n", a, a + 1, a + 2);
    printf("*a:0x%p, *(a+1):0x%p, *(a+2):0x%p\n", *a, *(a + 1), *(a + 2));
    printf("*a->age:%d, *(a+1)->age:%d, *(a+2)->age:%d\n", (*a)->age, (*(a + 1))->age, (*(a + 2))->age);
    bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), compare_3);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(elem_type); ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i]->name, a[i]->age);
    }
    fprintf(stdout, "********************************\n");
    free(stu5_);
    free(stu6_);
    free(stu7_);
    free(stu8_);
}
void sort_test_33()
{
    Student stu1 = {"Tom", 4};
    Student stu2 = {"Amy", 6};
    Student stu3 = {"Miguel", 5};
    Student stu4 = {"Jacob", 2};
    Student stu5 = {"Alex", 8};
    Student stu6 = {"Bob", 1};
    Student stu7 = {"John", 7};
    Student stu8 = {"Tony", 3};

    elem_type a[] = {&stu1, &stu2, &stu3, &stu4, &stu5, &stu6, &stu7, &stu8};
    bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), compare_3);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(long); ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i]->name, a[i]->age);
    }
    fprintf(stdout, "********************************\n");
}
void sort_test_333()
{
    elem_type stu1_ = (elem_type)malloc(sizeof(Student));
    stu1_->name = "Tom";
    stu1_->age = 4;
    elem_type stu2_ = (elem_type)malloc(sizeof(Student));
    stu2_->name = "Amy";
    stu2_->age = 6;
    elem_type stu3_ = (elem_type)malloc(sizeof(Student));
    stu3_->name = "Miguel";
    stu3_->age = 5;
    elem_type stu4_ = (elem_type)malloc(sizeof(Student));
    stu4_->name = "Jacob";
    stu4_->age = 2;
    elem_type stu5_ = (elem_type)malloc(sizeof(Student));
    stu5_->name = "Alex";
    stu5_->age = 8;
    elem_type stu6_ = (elem_type)malloc(sizeof(Student));
    stu6_->name = "Bob";
    stu6_->age = 1;
    elem_type stu7_ = (elem_type)malloc(sizeof(Student));
    stu7_->name = "John";
    stu7_->age = 7;
    elem_type stu8_ = (elem_type)malloc(sizeof(Student));
    stu8_->name = "Tony";
    stu8_->age = 3;

    elem_type a[] = {stu1_, stu2_, stu3_, stu4_, stu5_, stu6_, stu7_, stu8_};
    bubble_sort(a, (sort_index_type)(sizeof(a) / sizeof(elem_type)), compare_3);
    int i;
    for (i = 0; i < sizeof(a) / sizeof(elem_type); ++i)
    {
        fprintf(stdout, "%s: %d\n", a[i]->name, a[i]->age);
    }
    fprintf(stdout, "********************************\n");
    free(stu1_);
    free(stu2_);
    free(stu3_);
    free(stu4_);
    free(stu5_);
    free(stu6_);
    free(stu7_);
    free(stu8_);
}