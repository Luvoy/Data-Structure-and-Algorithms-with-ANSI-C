#include <stdlib.h>
#include <stdint.h>
// restrict the lengths of all types to avoid variety of length using different compilers
#include <inttypes.h>
// avoid mismatch between format str and type length in cross-platform environment
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

typedef uint8_t my_bool;
#define TRUE ((my_bool)1)
#define FALSE ((my_bool)0)
#define PRI_MY_BOOL PRIu8
/********************************************************/
//用法：一个.c文件只能用一次
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

// 如果还要用到T，就得再define一遍
// #define STACK_T int32_t
// #define STACK_T_NULL ((STACK_T)0)
/********************************************************/

typedef int64_t stack_size_type;

typedef struct Stack
{
    STACK_T *base; //
    STACK_T *top;  //指元素尾部, 而不是指向元素起始地址处
    stack_size_type size;
} Stack;

static my_bool stack_is_empty(const Stack *s)
{
    if (s->base >= s->top)
    {
        return TRUE;
    }
    return FALSE;
}

static my_bool stack_is_full(const Stack *s)
{
    if (s->base + (s->size) == s->top)
    {
        return TRUE;
    }
    return FALSE;
}

static stack_size_type stack_used_size(const Stack *s)
{
    assert(s != NULL);
    return s->top - s->base;
}

static stack_size_type stack_total_size(const Stack *s)
{
    assert(s != NULL);
    return s->size;
}

static Stack *stack_new_empty(stack_size_type size)
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s != NULL);
    s->base = (STACK_T *)calloc(size, sizeof(STACK_T));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s->base != NULL);
    s->top = s->base;
    s->size = size;
    return s;
}

static void stack_free(Stack **sp)
{
    assert(sp != NULL);
    assert(*sp != NULL);
    free((*sp)->base);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free((*sp));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *sp = NULL;
}

static void stack_clear(Stack *s)
{
    assert(s != NULL);
    memset(s->base, 0, s->size * sizeof(STACK_T));
    s->top = s->base;
}

static void stack_push(Stack *s, STACK_T elem, my_bool allow_auto_expan, cmp_ret_type (*cmp)(const STACK_T, const STACK_T))
{
    assert(s != NULL);
    if (cmp(elem, STACK_T_NULL) == 0)
    {
        return;
    }
    assert(s != NULL);
    if (stack_is_full(s))
    {
        if (allow_auto_expan)
        {
            fprintf(stderr, "NOTE: stack is full, trying to reallocate.\n");
            stack_size_type used = s->top - s->base;
            s->base = (STACK_T *)realloc(s->base, (s->size << 1) * sizeof(STACK_T));
            //             STACK_T *temp = (STACK_T *)malloc((s->size << 1) * sizeof(STACK_T ));
            // #ifdef DEBUG_ALLOC_FREE_COUNT
            //             g_alloc_count++;
            // #endif
            //             if (temp == NULL)
            //             {
            //                 fprintf(stderr, "ERROR: stack overflow!\n");
            //                 return;
            //             }
            //             memcpy(temp, s->base, (s->top - s->base) * sizeof(STACK_T ));
            //             free(s->base);
            // #ifdef DEBUG_ALLOC_FREE_COUNT
            //             g_free_count++;
            // #endif
            //             s->base = temp;
            s->top = s->base + used;
            s->size <<= 1;
        }
        else
        {
            fprintf(stderr, "ERROR: stack overflow!\n");
            return;
        }
    }
    *(s->top) = elem;
    s->top++;
}

static STACK_T stack_pop(Stack *s)
{
    assert(s != NULL);
    if (!stack_is_empty(s))
    {

        STACK_T ret = *(s->top - 1);
        *(s->top - 1) = STACK_T_NULL;
        s->top--;
        return ret;
    }
    else
    {
        fprintf(stderr, "ERROR: empty stack!\n");
        return STACK_T_NULL;
    }
}

static STACK_T stack_get_top(const Stack *s)
{
    assert(s != NULL);
    if (!stack_is_empty(s))
    {
        return *(s->top - 1);
    }
    else
    {
        fprintf(stderr, "ERROR: empty stack!\n");
        return STACK_T_NULL;
    }
}

static Stack *stack_new_from_elements(stack_size_type stack_size, stack_size_type elem_count, STACK_T *p)
{ /* Expected: size >= elements count
     Agree on: bottom element at first */
    if (stack_size < elem_count)
    {
        fprintf(stderr, "ERROR: expected: size >= elements count!\n");
        return NULL;
    }
    Stack *s = (Stack *)malloc(sizeof(Stack));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s != NULL);
    s->base = (STACK_T *)calloc(stack_size, sizeof(STACK_T));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s->base != NULL);
    stack_size_type i;
    for (i = 0; i < elem_count; ++i)
    {
        *(s->base + i) = *(p + i);
    }
    s->top = s->base + elem_count;
    s->size = stack_size;
    return s;
}

static Stack *stack_new_from_args(stack_size_type stack_size, stack_size_type elem_count, ...)
{ /* Expected: size >= elements count
     Agree on: bottom element at first */
    if (stack_size < elem_count)
    {
        fprintf(stderr, "Expected: size >= elements count!\n");
        return NULL;
    }
    Stack *s = (Stack *)malloc(sizeof(Stack));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s != NULL);
    s->base = (STACK_T *)calloc(stack_size, sizeof(STACK_T));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s->base != NULL);
    stack_size_type i;
    va_list vl;
    va_start(vl, elem_count);
    for (i = 0; i < elem_count; ++i)
    {
        *(s->base + i) = va_arg(vl, STACK_T);
    }
    va_end(vl);
    s->top = s->base + elem_count;
    s->size = stack_size;
    return s;
}

static void stack_print(
    const Stack *s,
    FILE *fp,
    const char *format,
    void (*elem_print)(FILE *, const STACK_T, cmp_ret_type (*)(const STACK_T, const STACK_T)),
    cmp_ret_type (*cmp)(const STACK_T, const STACK_T))
{
    assert(s != NULL);
    if (strcmp(format, "SINGLE_LINE") != 0 && strcmp(format, "MULTI_LINES") != 0)
    {
        fprintf(stderr, "ERROR: undefined format string: %s, expected SINGLE_LINE or MULTI_LINES.\n", format);
        return;
    }
    if (strcmp(format, "SINGLE_LINE") == 0)
    { /* may overflow */
        fprintf(fp, "Stack at: %p, size: %" PRId64 ", used: %" PRId64 ", content: ", s, s->size, stack_used_size(s));
        stack_size_type i;
        for (i = 0; s->base + i < s->top; ++i)
        {

            if (s->base + i == s->base)
            {
                elem_print(fp, *(s->base + i), cmp);
                fprintf(fp, "(base)");
            }
            else
            {
                fprintf(fp, " >> ");
                elem_print(fp, *(s->base + i), cmp);
            }
            if (s->base + i + 1 == s->top)
            {
                fprintf(fp, "(top)");
            }
        }

        for (; i < s->size; ++i)
        {
            if (i != 0)
                fprintf(fp, " >> STACK_T_NULL");
            else
                fprintf(fp, "STACK_T_NULL");
        }
        fprintf(fp, "\n");
    }
    if (strcmp(format, "MULTI_LINES") == 0)
    {
        fprintf(fp, "==================== Stack at %18p ====================\n", s);
        fprintf(fp, "================== used_size: %20" PRId64 " ==================\n", stack_used_size(s));
        fprintf(fp, "================= total_size: %20" PRId64 " ==================\n", s->size);
        stack_size_type i;
        for (i = s->size - 1; s->base + i >= s->top; i--)
        {
            fprintf(fp, "         STACK_T_NULL\n");
        }
        for (; s->base + i >= s->base; --i)
        {
            if (s->base + i + 1 == s->top)
            {
                fprintf(fp, "(top)  -> ");
            }
            if (s->base + i == s->base)
            {
                fprintf(fp, "(base) -> ");
            }
            if (s->base + i != s->top && s->base + i != s->base)
            {
                fprintf(fp, "        ");
            }
            elem_print(fp, *(s->base + i), cmp);
            fprintf(fp, "\n");
        }
        fprintf(fp, "========================= Stack Print: Over =========================\n");
    }
}

#undef STACK_T
#undef STACK_T_NULL