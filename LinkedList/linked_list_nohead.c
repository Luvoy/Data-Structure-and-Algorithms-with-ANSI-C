/* linked_list.c */
#include "linked_list_nohead.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

LinkedList linked_list_new(size_type size, ObjectFuncs *o)
{

    assert(size >= 0);
    Node *p_temp;
    LinkedList L;

    LinkedList L = NULL;
    if (size == 0)
    {
        return L;
    }
    else
    {
        LinkedList L = (LinkedList)calloc(1, sizeof(Node));
        assert(L);
        Node *p_temp = L;
    }

    while (size--)
    {
        p_temp->next = (Node *)calloc(1, sizeof(Node));
        assert(p_temp->next);
        p_temp = p_temp->next;
        p_temp->obj_funcs = o;
        p_temp->next = NULL;
    }

    return L;
}

size_type linked_list_length(const LinkedList L)
{
    Node *p_temp;
    size_type len;

    p_temp = L;

    if (!p_temp)
    {
        return 0;
    }
    for (len = 1; p_temp->next; p_temp = p_temp->next)
    {
        ++len;
    }
    return len;
}
