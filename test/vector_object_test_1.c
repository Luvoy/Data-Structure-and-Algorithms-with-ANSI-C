#include "vector_object_test.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#ifdef MY_DEBUG
#undef MY_DEBUG
#endif
typedef struct Student
{
    my_bool dynamic_memory_flag;
    const char *name;
    unsigned int age;
    float *san_wei;
    my_bool san_wei_memory_flag;
} Student;
static Student *student_calloc(size_type size)
{
    Student *stu = (Student *)calloc(1, size);
    stu->san_wei = (float *)calloc(3, sizeof(float));
    return stu;
}
static void student_free(Student *stu)
{

    if (stu->san_wei_memory_flag)
    {
        free(stu->san_wei);
        stu->san_wei = NULL;
    }

    if (stu->dynamic_memory_flag)
    {
        free(stu);
        stu = NULL; /* 没用*/
    }
}

static int student_compare(const Student *stu1, const Student *stu2)
{
    if (stu1->age != stu2->age)
    {
        return stu1->age - stu2->age;
    }
    else if (strcmp(stu1->name, stu2->name) != 0)
    {
        return strcmp(stu1->name, stu2->name);
    }
    else if (memcmp(stu1->san_wei, stu2->san_wei, 3 * sizeof(float)) != 0)
    {
        return memcmp(stu1->san_wei, stu2->san_wei, 3 * sizeof(float));
    }
    else
    {
        return 0;
    }
}

static void student_print_noend(FILE *stream, const Student *stu1)
{
    fprintf(stream, "name: %s, age: %d, san_wei: %.2fcm, %.2fcm, %.2fcm", stu1->name, stu1->age, *(stu1->san_wei + 0), *(stu1->san_wei + 1), *(stu1->san_wei + 2));
}
static void san_wei_deepcopy(float *src, float *dst, size_type num)
{
}
static void student_deepcopy(Student *stu_dst, const Student *stu_src)
{
    /*  stu_dst->dynamic_memory_flag = stu_src->dynamic_memory_flag;
    stu_dst->name = stu_src->name;
    stu_dst->age = stu_src->age;
    int i;
    for (i = 0; i < 3; ++i)
    {
        *(stu_dst->san_wei + i) = *(stu_src->san_wei + i);
    }
    stu_dst->san_wei_memory_flag = stu_src->san_wei_memory_flag; */
}
static ObjectFuncs stu_obj_funcs = {
    sizeof(Student *),
    sizeof(Student),
    student_calloc,
    student_free,
    student_compare,
    student_print_noend,
    student_deepcopy};
typedef Vector Vector_Student;
void vector_object_test_1(void)
{
    fprintf(stdout, "Generate data:\n");
    float *stu1_san_wei = calloc(3, sizeof(float));
    float *stu2_san_wei = calloc(3, sizeof(float));
    float *stu3_san_wei = calloc(3, sizeof(float));
    float *stu4_san_wei = calloc(3, sizeof(float));
    float *stu5_san_wei = calloc(3, sizeof(float));
    float *stu6_san_wei = calloc(3, sizeof(float));
    float *stu7_san_wei = calloc(3, sizeof(float));
    float *stu8_san_wei = calloc(3, sizeof(float));
    float *stu9_san_wei = calloc(3, sizeof(float));
    float *stu10_san_wei = calloc(3, sizeof(float));

    stu1_san_wei[0] = 101.0;
    stu2_san_wei[0] = 99.9;
    stu3_san_wei[0] = 81.0;
    stu4_san_wei[0] = 145.0;
    stu5_san_wei[0] = 98.0;
    stu1_san_wei[1] = 55.0;
    stu2_san_wei[1] = 55.5;
    stu3_san_wei[1] = 58.0;
    stu4_san_wei[1] = 81.0;
    stu5_san_wei[1] = 88.0;
    stu1_san_wei[2] = 84.0;
    stu2_san_wei[2] = 88.8;
    stu3_san_wei[2] = 86.0;
    stu4_san_wei[2] = 71.0;
    stu5_san_wei[2] = 98.0;

    stu6_san_wei[0] = 111.0;
    stu7_san_wei[0] = 110.0;
    stu8_san_wei[0] = 108.0;
    stu9_san_wei[0] = 102.0;
    stu10_san_wei[0] = 102.0;

    stu6_san_wei[1] = 61.0;
    stu7_san_wei[1] = 61.0;
    stu8_san_wei[1] = 62.0;
    stu9_san_wei[1] = 60.0;
    stu10_san_wei[1] = 60.0;

    stu6_san_wei[2] = 91.0;
    stu7_san_wei[2] = 89.0;
    stu8_san_wei[2] = 94.0;
    stu9_san_wei[2] = 89.0;
    stu10_san_wei[2] = 84.0;

    float stu11_san_wei[] = {1.0, 2.0, 3.0};
    float stu12_san_wei[] = {4.0, 5.0, 6.0};
    float stu13_san_wei[] = {6.0, 7.0, 8.0};
    float stu14_san_wei[] = {9.0, 10.0, 11.0};
    float stu15_san_wei[] = {12.0, 13.0, 14.0};
    float stu16_san_wei[] = {15.0, 16.0, 17.0};
    float stu17_san_wei[] = {18.0, 19.0, 20.0};
    float stu18_san_wei[] = {21.0, 22.0, 23.0};

    Student stu1 = {FALSE, "Julia", 33, stu1_san_wei, TRUE};
    Student stu2 = {FALSE, "Mine Fujiko", 49, stu2_san_wei, TRUE};
    Student stu3 = {FALSE, "Angelababy", 31, stu3_san_wei, TRUE};
    Student stu4 = {FALSE, "Arnold Schwarzenegger", 73, stu4_san_wei, TRUE};
    Student stu5 = {FALSE, "Tom", 22, stu5_san_wei, TRUE};
    Student stu6 = {FALSE, "Boa Hancock", 29, stu6_san_wei, TRUE};
    Student stu7 = {FALSE, "Ichihara Yuko", 32, stu7_san_wei, TRUE};
    Student stu8 = {FALSE, "Marikawa Shizuka", 25, stu8_san_wei, TRUE};

    Student *_stu9 = (Student *)calloc(1, sizeof(Student));
    _stu9->dynamic_memory_flag = TRUE;
    _stu9->name = "Himejima Akeno";
    _stu9->age = 19;
    _stu9->san_wei = stu9_san_wei;
    _stu9->san_wei_memory_flag = TRUE;

    Student *_stu10 = (Student *)calloc(1, sizeof(Student));
    _stu10->dynamic_memory_flag = TRUE;
    _stu10->name = "Katsura Kotonoha";
    _stu10->age = 17;
    _stu10->san_wei = stu10_san_wei;
    _stu10->san_wei_memory_flag = TRUE;

    Student *_stu11 = (Student *)calloc(1, sizeof(Student));
    _stu11->dynamic_memory_flag = TRUE;
    _stu11->name = "Amy";
    _stu11->age = 44;
    _stu11->san_wei = stu11_san_wei;
    _stu11->san_wei_memory_flag = FALSE;

    Student *_stu12 = (Student *)calloc(1, sizeof(Student));
    _stu12->dynamic_memory_flag = TRUE;
    _stu12->name = "Bill";
    _stu12->age = 54;
    _stu12->san_wei = stu12_san_wei;
    _stu12->san_wei_memory_flag = FALSE;

    Student *_stu13 = (Student *)calloc(1, sizeof(Student));
    _stu13->dynamic_memory_flag = TRUE;
    _stu13->name = "John";
    _stu13->age = 67;
    _stu13->san_wei = stu13_san_wei;
    _stu13->san_wei_memory_flag = FALSE;

    Student *_stu14 = (Student *)calloc(1, sizeof(Student));
    _stu14->dynamic_memory_flag = TRUE;
    _stu14->name = "Alex";
    _stu14->age = 26;
    _stu14->san_wei = stu14_san_wei;
    _stu14->san_wei_memory_flag = FALSE;

    Student *_stu15 = (Student *)calloc(1, sizeof(Student));
    _stu15->dynamic_memory_flag = TRUE;
    _stu15->name = "Riven";
    _stu15->age = 31;
    _stu15->san_wei = stu15_san_wei;
    _stu15->san_wei_memory_flag = FALSE;

    Student *_stu16 = (Student *)calloc(1, sizeof(Student));
    _stu16->dynamic_memory_flag = TRUE;
    _stu16->name = "Tony";
    _stu16->age = 29;
    _stu16->san_wei = stu16_san_wei;
    _stu16->san_wei_memory_flag = FALSE;

    Student *_stu17 = (Student *)calloc(1, sizeof(Student));
    _stu17->dynamic_memory_flag = TRUE;
    _stu17->name = "Sherry";
    _stu17->age = 17;
    _stu17->san_wei = stu17_san_wei;
    _stu17->san_wei_memory_flag = FALSE;

    Student *_stu18 = (Student *)calloc(1, sizeof(Student));
    _stu18->dynamic_memory_flag = TRUE;
    _stu18->name = "Conan";
    _stu18->age = 18;
    _stu18->san_wei = stu18_san_wei;
    _stu18->san_wei_memory_flag = FALSE;

    Student *data[] = {
        &stu1,
        &stu2,
        &stu3,
        &stu4,
        &stu5,
        &stu6,
        &stu7,
        &stu8,
        _stu9,
        _stu10,
        _stu11,
        _stu12,
        _stu13,
        _stu14,
        _stu15,
        _stu16,
        _stu17,
        _stu18,
    };
    fprintf(stdout, "Generate data: OK\n");
    fprintf(stdout, "Test vector_new:\n");
    fprintf(stdout, "Please input the size of vector (1-10)\n");
    int n = 5;
    Vector_Student *vs1 = vector_new(n, &stu_obj_funcs);
    fprintf(stdout, "create vector: OK\n");
    fprintf(stdout, "Test vector_index_assign:\n");
    int i;
    for (i = 0; i < n; ++i)
    {
        fprintf(stdout, "assigning %d...\n", i);
        vector_index_assign(vs1, i, data[i]);
    }
    fprintf(stdout, "assign complete\n");
    fprintf(stdout, "Test vector_length:\n");
    fprintf(stdout, "vs1 len: %d\n", vector_length(vs1));
    fprintf(stdout, "Test print:\n");
    vector_print(vs1, "MULTI_LINES");
    fprintf(stdout, "print over.\n");

    fprintf(stdout, "Test vector_new_from_objects:\n");
    Vector_Student *vs2 = vector_new_from_objects((void **)(data + n), 6, &stu_obj_funcs);
    fprintf(stdout, "vs2:\n");
    vector_print(vs2, "MULTI_LINES");

    fprintf(stdout, "Test vector_new_from_args:\n");
    Vector_Student *vs3 = vector_new_from_args(&stu_obj_funcs, 7, _stu12, _stu13, _stu14, _stu15, _stu16, _stu17, _stu18);
    fprintf(stdout, "vs3:\n");
    vector_print(vs3, "MULTI_LINES");

    fprintf(stdout, "Test vector_new_from_vector:\n");
    /*  Vector_Student *vs4 = vector_new_from_vector(vs1); */
    Vector_Student *vs4 = vs1;
    fprintf(stdout, "vs4:\n");
    vector_print(vs4, "MULTI_LINES");

    fprintf(stdout, "Test vector_length:\n");
    fprintf(stdout, "vs2 vector_length: %d\n", vector_length(vs2));
    fprintf(stdout, "vs3 vector_length: %d\n", vector_length(vs3));
    fprintf(stdout, "vs4 vector_length: %d\n", vector_length(vs4));

    fprintf(stdout, "Test vector_object_at:\n");
    fprintf(stdout, "vs2[1] is ");
    student_print_noend(stdout, vector_object_at(vs2, 1, FALSE));
    fprintf(stdout, "\n");

    fprintf(stdout, "vs3[-2] is ");
    student_print_noend(stdout, vector_object_at(vs3, -2, TRUE));
    fprintf(stdout, "\n");

    fprintf(stdout, "Test vector_left_search:\n");
    float stu_temp_san_wei[] = {99.9, 55.5, 88.8};
    Student stu_temp_1 = {FALSE, "Mine Fujiko", 49, stu_temp_san_wei};
    Student stu_temp_2 = {FALSE, "Mine Fujiko", 50, stu_temp_san_wei};
    fprintf(stdout, "try to find ");
    student_print_noend(stdout, &stu_temp_1);
    fprintf(stdout, " in vs1:\n");
    fprintf(stdout, "%d\n", vector_left_search(vs1, &stu_temp_1));
    fprintf(stdout, "try to find ");
    student_print_noend(stdout, &stu_temp_2);
    fprintf(stdout, " in vs1:\n");
    fprintf(stdout, "%d\n", vector_left_search(vs1, &stu_temp_2));

    fprintf(stdout, "Test vector_append:\nBefore append, vs2:\n");
    vector_print(vs2, "MULTI_LINES");
    vector_append(vs2, &stu_temp_1);
    fprintf(stdout, "after append, vs2:\n");
    vector_print(vs2, "MULTI_LINES");

    fprintf(stdout, "Test vector_pop:\nBefore pop, vs4:\n");
    vector_print(vs4, "MULTI_LINES");
    vector_pop(vs4);
    fprintf(stdout, "after pop, vs4:\n");
    vector_print(vs4, "MULTI_LINES");

    fprintf(stdout, "Test vector_pop_i:\n");
    vector_pop_i(vs4, 2);
    fprintf(stdout, "after pop_2, vs4:\n");
    vector_print(vs4, "MULTI_LINES");

    fprintf(stdout, "Test vector_insert:\n");
    vector_insert(vs4, 1, &stu_temp_2);
    fprintf(stdout, "after insert, vs4:\n");
    vector_print(vs4, "MULTI_LINES");

    fprintf(stdout, "Test vector_slice:\n");
    fprintf(stdout, "vs2, 0, 0, 1, is ");
    vector_print(vector_slice(vs2, 0, 0, 1, &stu_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vs2, -1, -1, 1, is ");
    vector_print(vector_slice(vs2, -1, -1, 1, &stu_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vs2, 1, 1, -1, is ");
    vector_print(vector_slice(vs2, 1, 1, -1, &stu_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vs2, -1, -1, -1, is ");
    vector_print(vector_slice(vs2, -1, -1, -1, &stu_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vs2, -4, -1, -2, is ");
    vector_print(vector_slice(vs2, -4, -1, -2, &stu_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vs2, -3, -1, 1, is ");
    vector_print(vector_slice(vs2, -3, -1, 1, &stu_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vs2, -1,-5, -1, is ");
    vector_print(vector_slice(vs2, -1, -5, -1, &stu_obj_funcs), "SINGLE_LINE");
    fprintf(stdout, "vs2, 4,2, -2, is ");
    vector_print(vector_slice(vs2, 4, 2, -2, &stu_obj_funcs), "SINGLE_LINE");

    fprintf(stdout, "Test vector_reverse:\nBefore reverse, vs2:\n");
    vector_print(vs2, "MULTI_LINES");
    vector_reverse(vs2, sizeof(Student *));
    vector_print(vs2, "MULTI_LINES");

    fprintf(stdout, "Test vector_deep_copy:copy vs4 to vs2\nBefore copy, vs2:\n");
    vector_print(vs2, "MULTI_LINES");
    vector_deep_copy(vs4, vs2);
    fprintf(stdout, "after copy, vs2:\n");
    vector_print(vs2, "MULTI_LINES");
    /* 注意! vs2可能不能用了*/
    fprintf(stdout, "Test vector_join:\nBefore join, vs3:\n");
    vector_print(vs3, "MULTI_LINES");
    vector_join(vs3, vs4);
    fprintf(stdout, "after join(vs3, vs4), vs3:\n");
    vector_print(vs3, "MULTI_LINES");

    fprintf(stdout, "Test free:\n");
    vector_free(vs1);
    vs1 = NULL;
    vector_free(vs2);
    vs2 = NULL;
    vector_free(vs3);
    vs3 = NULL;
    /* vector_free(vs4);
    vs4 = NULL; */
    fprintf(stdout, "free over\n");
    fprintf(stdout, "Free original data:\n");
    free(stu1_san_wei);
    fprintf(stdout, "free over\n");
    free(stu2_san_wei);
    fprintf(stdout, "free over\n");
    free(stu3_san_wei);
    fprintf(stdout, "free over\n");
    free(stu4_san_wei);
    fprintf(stdout, "free over\n");
    free(stu5_san_wei);
    fprintf(stdout, "free over\n");
    free(stu6_san_wei);
    fprintf(stdout, "free over\n");
    free(stu7_san_wei);
    fprintf(stdout, "free over\n");
    free(stu8_san_wei);
    fprintf(stdout, "free over\n");
    free(stu9_san_wei);
    fprintf(stdout, "free over\n");
    free(stu10_san_wei);
    fprintf(stdout, "free over\n");
    free(_stu9);
    fprintf(stdout, "free over\n");
    free(_stu10);
    fprintf(stdout, "free over\n");
    free(_stu11);
    fprintf(stdout, "free over\n");
    free(_stu12);
    fprintf(stdout, "free over\n");
    free(_stu13);
    fprintf(stdout, "free over\n");
    free(_stu14);
    fprintf(stdout, "free over\n");
    free(_stu15);
    fprintf(stdout, "free over\n");
    free(_stu16);
    fprintf(stdout, "free over\n");
    free(_stu17);
    fprintf(stdout, "free over\n");
    free(_stu18);
    fprintf(stdout, "free over\n");
    fprintf(stdout, "Free original data: OK\n");
}
