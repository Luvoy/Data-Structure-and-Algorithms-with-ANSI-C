#ifndef _TYPE_H_
#define _TYPE_H_
#include <stdlib.h>

typedef int element_type;
extern element_type ERROR_ELEMENT;

#define d element_print_format_str

typedef int size_type;
extern size_type ERROR_SIZE;
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef MY_my_bool
#define MY_my_bool
typedef int my_bool;
my_bool TRUE;
my_bool FALSE;
#endif

extern int compare(element_type a, element_type b);

#endif