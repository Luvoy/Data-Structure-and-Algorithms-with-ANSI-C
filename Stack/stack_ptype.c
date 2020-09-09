#include "stack_ptype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>

extern my_bool stack_is_empty(const Stack *s)
{
    if (s->base >= s->top)
    {
        return TRUE;
    }
    return FALSE;
}

extern my_bool stack_is_full(const Stack *s)
{
    if (s->base + (s->size) == s->top)
    {
        return TRUE;
    }
    return FALSE;
}

extern stack_size_type stack_used_size(const Stack *s)
{
    assert(s != NULL);
    return s->top - s->base;
}

extern stack_size_type stack_total_size(const Stack *s)
{
    assert(s != NULL);
    return s->size;
}

extern Stack *stack_new_empty(stack_size_type size)
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s != NULL);
    s->base = (void **)calloc(size, sizeof(void *));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s->base != NULL);
    s->top = s->base;
    s->size = size;
    return s;
}

extern void stack_free(Stack **sp)
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

extern void stack_clear(Stack *s)
{
    assert(s != NULL);
    memset(s->base, 0, s->size * sizeof(void *));
    s->top = s->base;
}

extern void stack_push(Stack *s, void *elem, my_bool allow_auto_expan, cmp_ret_type (*cmp)(const void *, const void *))
{
    assert(s != NULL);
    if (cmp(elem, NULL) == 0)
    {
        return;
    }
    if (stack_is_full(s))
    {
        if (allow_auto_expan)
        {
            fprintf(stderr, "NOTE: stack is full, trying to reallocate.\n");
            stack_size_type used = s->top - s->base;
            s->base = (void **)realloc(s->base, (s->size << 1) * sizeof(void *));
            //             void **temp = (void **)malloc((s->size << 1) * sizeof(void *));
            // #ifdef DEBUG_ALLOC_FREE_COUNT
            //             g_alloc_count++;
            // #endif
            //             if (temp == NULL)
            //             {
            //                 fprintf(stderr, "ERROR: stack overflow!\n");
            //                 return;
            //             }
            //             memcpy(temp, s->base, (s->top - s->base) * sizeof(void *));
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

extern void *stack_pop(Stack *s)
{
    assert(s != NULL);
    if (!stack_is_empty(s))
    {

        void *ret = *(s->top - 1);
        *(s->top - 1) = NULL;
        s->top--;
        return ret;
    }
    else
    {
        fprintf(stderr, "ERROR: empty stack!\n");
        return NULL;
    }
}

extern void *stack_get_top(const Stack *s)
{
    assert(s != NULL);
    if (!stack_is_empty(s))
    {
        return *(s->top - 1);
    }
    else
    {
        fprintf(stderr, "ERROR: empty stack!\n");
        return NULL;
    }
}

extern Stack *stack_new_from_elements(stack_size_type stack_size, stack_size_type elem_count, void **p)
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
    s->base = (void **)calloc(stack_size, sizeof(void *));
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

extern Stack *stack_new_from_args(stack_size_type stack_size, stack_size_type elem_count, ...)
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
    s->base = (void **)calloc(stack_size, sizeof(void *));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s->base != NULL);
    stack_size_type i;
    va_list vl;
    va_start(vl, elem_count);
    for (i = 0; i < elem_count; ++i)
    {
        *(s->base + i) = va_arg(vl, void *);
    }
    va_end(vl);
    s->top = s->base + elem_count;
    s->size = stack_size;
    return s;
}

extern void stack_print(const Stack *s, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *))
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
                elem_print(fp, *(s->base + i));
                fprintf(fp, "(base)");
            }
            else
            {
                fprintf(fp, " >> ");
                elem_print(fp, *(s->base + i));
            }
            if (s->base + i + 1 == s->top)
            {
                fprintf(fp, "(top)");
            }
        }

        for (; i < s->size; ++i)
        {
            if (i != 0)
                fprintf(fp, " >> NULL");
            else
                fprintf(fp, "NULL");
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
            fprintf(fp, "         NULL\n");
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
            elem_print(fp, *(s->base + i));
            fprintf(fp, "\n");
        }
        fprintf(fp, "========================= Stack Print: Over =========================\n");
    }
}
