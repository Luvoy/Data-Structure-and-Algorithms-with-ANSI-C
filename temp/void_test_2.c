#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
/* 方案1：先测试不用这种alloc函数，定义几个student，直接赋初值，然后创建vector。
方案2：在vector_new 的时候，调用object_alloc，object_alloc返回结果为void *
方案3：在vecotr_new 的时候，调用object_alloc，object_alloc返回结果为Student *
方案4：在vecor——new的时候，传入函数指针返回结果为void*
方案5：在vector——new的时候，传入函数指针，返回结果为Student *`
这里返回值Student*,
*/
/*另外还需要测试 set函数接受void* ，我传入一个student*行吗。*/
typedef struct Student
{
    const char *name;
    unsigned int age;
} Student;
typedef void *(*p_func)(int);
Student *object_alloc_3(int);
void test1()
{
    fprintf(stdout, "test 1:\n");
    int count = 3;
    void **a = (void **)calloc(count, sizeof(Student *));
    int i;
    for (i = 0; i < count; ++i)
    {
        *(a + i) = (Student *)calloc(1, sizeof(Student));
    }

    Student stu0 = {"Tom", 18};
    Student stu1 = {"Amy", 22};
    Student stu2 = {"John", 24};

    *(a + 0) = &stu0;
    *(a + 1) = &stu1;
    *(a + 2) = &stu2;
    fprintf(stdout, "a value: 0x%p\n", a);
    for (i = 0; i < count; ++i)
    {
        fprintf(stdout, "index: %d, name: %s, age: %d\n", i, ((Student *)(*(a + i)))->name, ((Student *)(*(a + i)))->age);
    }
    free(a);

    a = NULL;
}
void *object_alloc_2(int object_size)
{
    return (void *)calloc(1, object_size);
}
void test2()
{
    int i;
    int count = 3;
    fprintf(stdout, "test 2:\n");

    void **b = (void **)calloc(count, sizeof(void *));
    for (i = 0; i < count; ++i)
    {
        *(b + i) = (void *)object_alloc_2(sizeof(Student *));
    }
    Student stu0 = {"Yasha", 18};
    Student stu1 = {"Coco", 22};
    Student stu2 = {"Bjon", 24};
    *(b + 0) = &stu0;
    *(b + 1) = &stu1;
    *(b + 2) = &stu2;
    fprintf(stdout, "b value: 0x%p\n", b);
    for (i = 0; i < count; ++i)
    {
        fprintf(stdout, "index: %d, name: %s, age: %d\n", i, ((Student *)(*(b + i)))->name, ((Student *)(*(b + i)))->age);
    }
    for (i = 0; i < count; ++i)
    {
        free(*(b + i));
        *(b + i) = NULL;
    }
    free(b);
    b = NULL;
}
Student *object_alloc_3(int object_size)
{ /*声明需要和定义一致*/
    return (Student *)calloc(1, object_size);
}
void test3()
{
    int i;
    int count = 3;
    fprintf(stdout, "test 3:\n");

    void **b = (void **)calloc(count, sizeof(void *));
    for (i = 0; i < count; ++i)
    {
        *(b + i) = (void *)object_alloc_3(sizeof(Student *));
    }
    Student stu0 = {"Yasha", 18};
    Student stu1 = {"Coco", 22};
    Student stu2 = {"Bjon", 24};
    *(b + 0) = &stu0;
    *(b + 1) = &stu1;
    *(b + 2) = &stu2;
    fprintf(stdout, "b value: 0x%p\n", b);
    for (i = 0; i < count; ++i)
    {
        fprintf(stdout, "index: %d, name: %s, age: %d\n", i, ((Student *)(*(b + i)))->name, ((Student *)(*(b + i)))->age);
    }
    for (i = 0; i < count; ++i)
    {
        free(*(b + i));
        *(b + i) = NULL;
    }
    free(b);
    b = NULL;
}
void test4(int count, void *(*func_object_alloc)(int), int object_size)
{
    printf("test4\n");
    void **p = (void **)calloc(count, sizeof(void *));
    assert(p != NULL);
    int i;
    for (i = 0; i < count; ++i)
    {
        printf("hahaa\n");
        *(p + i) = func_object_alloc(object_size);
    }
    Student stu0 = {"Yasha", 18};
    Student stu1 = {"Coco", 22};
    Student stu2 = {"Bjon", 24};
    *(p + 0) = &stu0;
    *(p + 1) = &stu1;
    *(p + 2) = &stu2;
    fprintf(stdout, "p value: 0x%p\n", p);
    for (i = 0; i < count; ++i)
    {
        fprintf(stdout, "index: %d, name: %s, age: %d\n", i, ((Student *)(*(p + i)))->name, ((Student *)(*(p + i)))->age);
    }
    for (i = 0; i < count; ++i)
    {
        free(*(p + i));
        *(p + i) = NULL;
    }
    free(p);
    p = NULL;
}
int main(int argc, char const *argv[])
{
    test1();
    test2();
    test3();
    test4(3, object_alloc_2, sizeof(Student *));
    /* test4(3, object_alloc_3, sizeof(Student *)); */
    test4(3, (p_func)(object_alloc_3), sizeof(Student *));
    /*声明函数指针.然后强制类型转换 */
    return 0;
}
