typedef struct Student
{
    const char *name;
    unsigned int age;
} Student;
#define T Student
#define NULL_ELEM \
    (Student) { NULL, 0 }
#include "deque_standalone.h"