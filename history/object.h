#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <stdlib.h>
#include <stdio.h>

typedef int size_type;
#define ERROR_SIZE -1;

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef void *(*type_p_func_object_alloc)(size_type object_pointer_size);
typedef void (*type_p_func_object_free)(void **object);
typedef int (*type_p_func_object_compare)(const void *obj1, const void *obj2);
typedef void (*type_p_func_object_print)(FILE *stream, void *object);
typedef void (*type_p_func_object_deepcopy)(void *obj_dst, const void *obj_src, ...);
typedef struct ObjectFuncs
{
    size_type object_pointer_size; /* 一般是4 */
    size_type object_size;         /* sizeof(a struct),sizeof(int),..., .etc */
    type_p_func_object_alloc p_func_object_alloc;
    type_p_func_object_free p_func_object_free; /* doing nothing is the best, 除非有方法分辨某个变量是堆上的还是栈上的 */
    type_p_func_object_compare p_func_object_compare;
    type_p_func_object_print p_func_object_print;
    type_p_func_object_deepcopy p_func_object_deepcopy;
} ObjectFuncs;

#endif
