#include <stdlib.h>
#include <stdio.h>
#include "void_p_test.h"

void void_test_direct_alloc(void)
{
    int count = 3;
    void **a = (void **)calloc(count, sizeof(void *));
    int i;
    for (i = 0; i < count; i++)
    {
        *(a + i) = (Student *)calloc(1, sizeof(Student));
        /*可以把一个非void指针赋值到void指针, 也就是void*可以作为左值*/
    }

    Student stu1 = {"Tom",
                    18};
    Student stu2 = {"Amy",
                    20};
    Student stu3 = {"Alex",
                    33};

    *(a + 0) = &stu1;
    *(a + 1) = &stu2;
    *(a + 2) = &stu3;
    /*可以把一个非void指针赋值到void指针, 也就是void*可以作为左值*/
    fprintf(stderr, "a is: 0x%p\n", a);
    for (i = 0; i < count; i++)
    {
        fprintf(stderr, "index: %d, name: %s, age: %d\n", i, ((Student *)(*(a + i)))->name, ((Student *)(*(a + i)))->age);
    } /* 但是作为右值,必须强制类型转换 */
}