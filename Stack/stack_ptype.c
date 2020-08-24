#include "stack_ptype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

extern my_bool stack_is_empty(const Stack *s)
{
    if (s->base > s->top && s->size == 0)
    {
        return TRUE;
    }
    return FALSE;
}

extern my_bool stack_is_full(const Stack *s)
{
    if (s->base + (s->size - 1) == s->top)
    {
        return TRUE;
    }
    return FALSE;
}

extern stack_size_type stack_used_size(const Stack *s)
{
    assert(s != NULL);
    return (s->top + 1 - s->base) / sizeof(void *);
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
    s->top = s->base - 1;
    s->size = size;
    return s;
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
        s->base[i] = *(p + i);
    }
    s->top = s->base + elem_count - 1;
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
        s->base[i] = va_arg(vl, void *);
    }
    va_end(vl);
    s->top = s->base + elem_count - 1;
    s->size = stack_size;
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
    memset(s->base, 0, s->size);
}

extern void stack_push(Stack *s, const void *elem)
{
    assert(s != NULL);
    if (stack_is_full(s))
    {
        fprintf(stderr, "NOTE: stack is full, trying to reallocate.");
        s->base = (void **)realloc(s->base, s->size << 1);
        if (!s->base)
        {
            fprintf(stderr, "ERROR: stack overflow!\n");
        }
    }
    else
    {
        *(s->top + 1) = elem;
        s->top++;
    }
}

extern void *stack_pop(Stack *s)
{
    assert(s != NULL);
    if (!stack_is_empty(s))
    {
        if (s->top >= s->base)
        {
            void *ret = *(s->top);
            *(s->top) = NULL;
            s->top--;
            return ret;
        }
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
        if (s->top >= s->base)
        {
            return *(s->top);
        }
    }
    else
    {
        fprintf(stderr, "ERROR: empty stack!\n");
        return NULL;
    }
}

extern void stack_print(const Stack *s, FILE *fp, const char *format, void (*elem_print)(FILE *, const void *))
{
    assert(s != NULL);
    if (strcmp(format, "SINGLE_LINE") != 0 || strcmp(format, "MULTI_LINES") != 0)
    {
        fprintf(stderr, "ERROR: undefined format string: %s, expected SINGLE_LINE or MULTI_LINES.\n", format);
        return;
    }
    if (strcmp(format, "SINGLE_LINE") == 0)
    { /* may overflow */
        fprintf(fp, "Stack at: %p, size: %d, used: %d", s, s->size, stack_used_size(s));
        stack_size_type i;
        for (i = 0; s->base + i <= s->top; ++i)
        {

            if (s->base + i == s->base)
            {
                elem_print(fp, s->base + i);
                fprintf(fp, "(base)");
            }
            else
            {
                fprintf(fp, " >> ");
                elem_print(fp, s->base + i);
            }
            if (s->base + i == s->top)
            {
                fprintf(fp, "(top)");
            }
        }
        for (; i < s->size; ++i)
        {
            fprintf(fp, " >> NULL");
        }
        fprintf(fp, "\n");
    }
    if (strcmp(format, "MULTI_LINES") == 0)
    {
        fprintf(fp, "==================== Stack at %18p ====================\n", s);
        fprintf(fp, "================== used_size: %20llu ==================\n", stack_used_size(s));
        fprintf(fp, "================= total_size: %20llu ==================\n", s->size);
        stack_size_type i;
        for (i = s->size - 1; s->base + i > s->top; i--)
        {
            fprintf(fp, "        NULL\n");
        }
        for (; s->base + i >= s->base; --i)
        {
            if (s->base + i == s->top)
            {
                fprintf(fp, "(top)  -> ");
            }
            else if (s->base + i == s->base)
            {
                fprintf(fp, "(base) -> ");
            }
            else
            {
                fprintf(fp, "        ");
            }
            elem_print(fp, s->base + i);
            fprintf(fp, "\n");
        }
        fprintf(fp, "========================= Stack Print: Over =========================\n");
    }
}