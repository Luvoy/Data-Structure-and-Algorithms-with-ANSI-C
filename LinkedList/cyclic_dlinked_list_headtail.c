#include "cyclic_dlinked_list_headtail.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static DNode *_node_alloc(void)
{
    DNode *p = (DNode *)calloc(1, sizeof(DNode));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(p);
    return p;
}

static DNode *node_new(void *data)
{
    DNode *p = _node_alloc();
    assert(p);
    p->data = data;
    p->next = NULL;
    p->prev = NULL;
    return p;
}

static void node_insert(DNode *p_node_left, DNode *p_node_new, DNode *p_node_right)
{ /*必须确认插入新节点不会对其他数据造成影响, 比如要插入的新节点原来后面还有很多节点时,会导致内存泄漏*/
    if (p_node_left == NULL && p_node_right == NULL)
    {
        fprintf(stderr, "Error, two nodes cannnot be NULL at the same time.\n");
    }
    if (p_node_new == NULL)
    {
        fprintf(stderr, "Error, middle node can not be NULL!\n");
    }
    p_node_new->prev = p_node_left;
    p_node_new->next = p_node_right;
    if (p_node_left != NULL)
    {
        p_node_left->next = p_node_new;
    }
    if (p_node_right != NULL)
    {
        p_node_right->prev = p_node_new;
    }
}

extern my_bool cyclic_dlinked_list_is_empty(const DLinkedList DL)
{
    assert(DL != NULL);
    if (DL->size == 0 || DL->head == NULL || DL->tail == NULL)
    {
        return TRUE;
    }
    return FALSE;
}

extern DLinkedList cyclic_dlinked_list_new(linked_list_size_type size)
{
    DLinkedList DL = (DLinkedList)malloc(sizeof(DNodeGuard));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(DL);
    DL->size = size;
    linked_list_index_type i;
    DNode *prev_node = NULL;
    for (i = 0; i < size; ++i)
    {
        DNode *temp = node_new(NULL);
        assert(temp);
        temp->prev = prev_node;
        if (prev_node != NULL)
        {
            prev_node->next = temp;
        }
        prev_node = temp;
        if (i == 0)
        {
            DL->head = temp;
        }
        if (i == size - 1)
        {
            DL->tail = temp;
            temp->next = DL->head;
            DL->head->prev = DL->tail;
        }
    }
    return DL;
}

extern DLinkedList cyclic_dlinked_list_new_from_args(linked_list_size_type size, ...)
{
    DLinkedList DL = (DLinkedList)malloc(sizeof(DNodeGuard));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(DL);
    DL->size = size;
    linked_list_index_type i;
    DNode *prev_node = NULL;
    va_list vl;
    va_start(vl, size);
    for (i = 0; i < size; ++i)
    {
        DNode *temp = node_new(va_arg(vl, void *));
        assert(temp);
        temp->prev = prev_node;
        if (prev_node != NULL)
        {
            prev_node->next = temp;
        }
        prev_node = temp;
        if (i == 0)
        {
            DL->head = temp;
        }
        if (i == size - 1)
        {
            DL->tail = temp;
            temp->next = DL->head;
            DL->head->prev = DL->tail;
        }
    }
    return DL;
}

extern DLinkedList cyclic_dlinked_list_new_from_elements(void **p, linked_list_size_type size)
{
    DLinkedList DL = (DLinkedList)malloc(sizeof(DNodeGuard));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(DL);
    DL->size = size;
    linked_list_index_type i;
    DNode *prev_node = NULL;
    for (i = 0; i < size; ++i)
    {
        DNode *temp = node_new(*(p + i));
        assert(temp);
        temp->prev = prev_node;
        if (prev_node != NULL)
        {
            prev_node->next = temp;
        }
        prev_node = temp;
        if (i == 0)
        {
            DL->head = temp;
        }
        if (i == size - 1)
        {
            DL->tail = temp;
            temp->next = DL->head;
            DL->head->prev = DL->tail;
        }
    }
    return DL;
}

extern void cyclic_dlinked_list_print(const DLinkedList DL, const char *format, void (*elem_print)(FILE *, const void *))
{
    assert(DL);
    assert(format);
    if (strcmp(format, "SINGLE_LINE") != 0 && strcmp(format, "MULTI_LINES") != 0)
    {
        fprintf(stderr, "Undefined format str: %s! Expected \"SINGLE_LINE\" or \"MULTI_LINES\"!\n", format);
        return;
    }
    linked_list_index_type i;
    DNode *temp = DL->head;
    while (TRUE)
    {
        elem_print(stdout, temp->data);
        temp = temp->next;
        if (temp != DL->head)
        {
            if (strcmp(format, "SINGLE_LINE") == 0)
            {
                fprintf(stdout, " <-> ");
            }
            if (strcmp(format, "MULTI_LINES") == 0)
            {
                fprintf(stdout, " <->\n");
            }
        }
        else
        {
            fprintf(stdout, " <-> [start]\n");
            return;
        }
    }
}

extern void cyclic_dlinked_list_free(DLinkedList *DLP)
{
    assert(DLP);
    assert(*DLP);
    DNode *slow = *DLP;
    DNode *fast = (*DLP)->head;
    while (TRUE)
    {
        slow = fast;
        fast = fast->next;
        free(slow);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
        if (fast == (*DLP)->head)
        {
            break;
        }
    }
    free(*DLP);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *DLP = NULL;
    return;
}

extern linked_list_size_type cyclic_dlinked_list_length(const DLinkedList DL)
{
    assert(DL);
    return DL->size;
}

extern void *cyclic_dlinked_list_right_delete_objects(DLinkedList DL, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *))
{
    if (n <= 0)
    {
        fprintf(stderr, "Error number of elem to be delete, expected n>0!\n");
        return NULL;
    }
    assert(DL);
    linked_list_size_type count = 0;
    DNode *temp = DL->tail;
    void *ret = NULL;
    linked_list_index_type i;
    for (i = DL->size - 1; i >= 0; --i)
    {
        if (elem_cmp(temp->data, elem) == 0)
        {
            if (temp == DL->tail)
            {
                DL->tail = temp->prev;
            }
            if (temp == DL->head)
            {
                DL->head = temp->next;
            }
            if (temp->prev != NULL)
            {
                temp->prev->next = temp->next;
            }
            if (temp->next != NULL)
            {
                temp->next->prev = temp->prev;
            }
            ret = temp->data;
            free(temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            count++;
            DL->size--;
            if (count == n)
            {
                return ret;
            }
        }
        temp = temp->prev;
    }
}

extern void *cyclic_dlinked_list_left_delete_objects(DLinkedList DL, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *))
{
    if (n <= 0)
    {
        fprintf(stderr, "Error number of elem to be delete, expected n>0!\n");
        return NULL;
    }
    assert(DL);
    linked_list_size_type count = 0;
    DNode *temp = DL->head;
    void *ret = NULL;
    linked_list_size_type temp_size = DL->size;
    linked_list_index_type i;
    for (i = 0; i < temp_size; ++i)
    {
        if (elem_cmp(temp->data, elem) == 0)
        {
            if (temp == DL->tail)
            {
                DL->tail = temp->prev;
            }
            if (temp == DL->head)
            {
                DL->head = temp->next;
            }
            if (temp->prev != NULL)
            {
                temp->prev->next = temp->next;
            }
            if (temp->next != NULL)
            {
                temp->next->prev = temp->prev;
            }
            ret = temp->data;
            free(temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            count++;
            DL->size--;
            if (count == n)
            {
                return ret;
            }
        }
        temp = temp->next;
    }
}

extern linked_list_index_type cyclic_dlinked_list_right_search(const DLinkedList DL, const void *elem, int (*elem_cmp)(const void *, const void *))
{
    assert(DL);
    linked_list_index_type ret = DL->size - 1;
    DNode *temp = DL->tail;
    do
    {
        if (elem_cmp(temp->data, elem) == 0)
        {
            return ret;
        }
        temp = temp->prev;
        ret--;
    } while (temp != DL->tail);
    return -1;
}

extern linked_list_index_type cyclic_dlinked_list_left_search(const DLinkedList DL, const void *elem, int (*elem_cmp)(const void *, const void *))
{
    assert(DL);
    linked_list_index_type ret = 0;
    DNode *temp = DL->head;
    do
    {
        if (elem_cmp(temp->data, elem) == 0)
        {
            return ret;
        }
        temp = temp->next;
        ret++;
    } while (temp != DL->head);
    return -1;
}

extern my_bool cyclic_dlinked_list_contains(const DLinkedList DL, void *elem, int (*elem_cmp)(const void *, const void *))
{
    if (cyclic_dlinked_list_left_search(DL, elem, elem_cmp) == -1)
    {
        return FALSE;
    }
    return TRUE;
}

extern void cyclic_dlinked_list_append_object(DLinkedList DL, void *elem)
{
    assert(DL);
    DNode *temp = node_new(elem);
    DL->size++;
    if (DL->tail != NULL)
    {
        DL->tail->next = temp;
    }
    else
    {
        DL->tail = temp;
    }
    if (DL->head != NULL)
    {
        DL->head->prev = temp;
    }
    else
    {
        DL->head = temp;
    }
    temp->next = DL->head;
    temp->prev = DL->tail;
    DL->tail = temp;
    return;
}

extern void cyclic_dlinked_list_join(DLinkedList DL_left, DLinkedList DL_right)
{
    if (DL_left == NULL || DL_right == NULL)
    {
        return;
    }
    if (cyclic_dlinked_list_is_empty(DL_left) || cyclic_dlinked_list_is_empty(DL_right))
    {
        return;
    }
    DL_left->tail->next = DL_right->head;
    DL_right->head->prev = DL_left->tail;
    DL_left->size += DL_right->size;
    DL_right->tail->next = DL_left->head;
    DL_left->head->prev = DL_right->tail;
    free(DL_right);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
}

extern void *cyclic_dlinked_list_pop(DLinkedList DL)
{
    assert(DL);
    if (DL->size <= 0)
    {
        fprintf(stderr, "Empty DLinkedList, nothing to pop!\n");
        return NULL;
    }
    DL->size--;
    void *ret = DL->tail->data;
    DNode *temp = DL->tail->prev;
    temp->next = DL->head;
    DL->head->prev = temp;
    free(DL->tail);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    DL->tail = temp;
    return ret;
}

extern void *cyclic_dlinked_list_object_at(const DLinkedList DL, linked_list_index_type index, my_bool allow_negative_index)
{
    assert(DL);
    if (allow_negative_index)
    {
        if (index < -(DL->size) || index >= DL->size)
        {
            fprintf(stderr, "Index out of range, index = %d, allowed: %d ~ %d\n", index, -(DL->size), DL->size - 1);
            return NULL;
        }
    }
    else
    {
        if (index < 0 || index >= DL->size)
        {

            fprintf(stderr, "Index out of range, index = %d, allowed: 0 ~ %d\n", index, DL->size - 1);
            return NULL;
        }
    }
    linked_list_index_type real_index = index > 0 ? index : DL->size + index;
    linked_list_index_type i = real_index < DL->size / 2 ? 0 : DL->size - 1;
    DNode *temp = real_index < DL->size / 2 ? DL->head : DL->tail;
    while (TRUE)
    {
        if (i == real_index)
        {
            return temp->data;
        }
        temp = real_index < DL->size / 2 ? temp->next : temp->prev;
        i = real_index < DL->size / 2 ? i + 1 : i - 1;
    }
}

extern void cyclic_dlinked_list_insert_object(DLinkedList DL, linked_list_index_type index, void *data)
{
    assert(DL != NULL);
    DNode *new = node_new(data);
    status ret_status = cyclic_dlinked_list_insert_node(DL, index, new);
    if (ret_status == -1)
    {
        free(new);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
    }
}

extern void cyclic_dlinked_list_insert_linked_list(DLinkedList DL_src, linked_list_index_type index, DLinkedList *DL_new)
{ /*将B插入A，B的头节点相关信息会被free*/
    assert(DL_new);
    if (cyclic_dlinked_list_is_empty((*DL_new)) || (*DL_new) == NULL)
    {
        return;
    }
    if (cyclic_dlinked_list_is_empty(DL_src) || (*DL_new) == NULL)
    {
        fprintf(stderr, "Error, source cyclic_dlinked_list is empty or null!\n");
    }
    if (index == DL_src->size)
    {
        cyclic_dlinked_list_join(DL_src, (*DL_new));
        return;
    }
    if (index < 0 || index > DL_src->size)
    {
        fprintf(stderr, "Index out of range, index = %d, allowed: 0 ~ %d\n", index, DL_src->size - 1);
        return;
    }
    linked_list_index_type i = index < DL_src->size / 2 ? 0 : DL_src->size - 1;
    DNode *temp = index < DL_src->size / 2 ? DL_src->head : DL_src->tail;
    while (TRUE)
    {
        if (i == index)
        {
            if (temp->prev != NULL)
            {
                temp->prev->next = (*DL_new)->head;
            }

            (*DL_new)->head->prev = temp->prev;
            (*DL_new)->tail->next = temp;
            temp->prev = (*DL_new)->tail;
            if (index == 0)
            {
                DL_src->head = (*DL_new)->head;
            }
            DL_src->size += (*DL_new)->size;
            free((*DL_new));
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            *DL_new = NULL;
            return;
        }
        temp = index < DL_src->size / 2 ? temp->next : temp->prev;
        i = index < DL_src->size / 2 ? i + 1 : i - 1;
    }
}

extern status cyclic_dlinked_list_insert_node(DLinkedList DL, linked_list_index_type index, DNode *p_new_node)
{
    if (index < 0 || index >= DL->size)
    {
        fprintf(stderr, "Index out of range, index = %d, allowed: 0 ~ %d\n", index, DL->size - 1);
        return -1;
    }
    linked_list_index_type i = index < DL->size / 2 ? 0 : DL->size - 1;
    DNode *temp = index < DL->size / 2 ? DL->head : DL->tail;
    while (TRUE)
    {
        if (i == index)
        {
            node_insert(temp->prev, p_new_node, temp);
            return 1;
        }
        temp = index < DL->size / 2 ? temp->next : temp->prev;
        i = index < DL->size / 2 ? i + 1 : i - 1;
    }
}

extern void *cyclic_dlinked_list_index_assign(const DLinkedList DL, linked_list_index_type index, my_bool allow_negative_index, void *elem)
{
    assert(DL);
    if (allow_negative_index)
    {
        if (index < -(DL->size) || index >= DL->size)
        {
            fprintf(stderr, "Index out of range, index = %d, allowed: %d ~ %d\n", index, -(DL->size), DL->size - 1);
            return NULL;
        }
    }
    else
    {
        if (index < 0 || index >= DL->size)
        {

            fprintf(stderr, "Index out of range, index = %d, allowed: 0 ~ %d\n", index, DL->size - 1);
            return NULL;
        }
    }
    linked_list_index_type real_index = index > 0 ? index : DL->size + index;
    linked_list_index_type i = real_index < DL->size / 2 ? 0 : DL->size - 1;
    DNode *temp = real_index < DL->size / 2 ? DL->head : DL->tail;
    while (TRUE)
    {
        if (i == real_index)
        {
            void *ret = temp->data;
            temp->data = elem;
            return ret;
        }
        temp = real_index < DL->size / 2 ? temp->next : temp->prev;
        i = real_index < DL->size / 2 ? i + 1 : i - 1;
    }
}

extern void *cyclic_dlinked_list_pop_i(DLinkedList DL, linked_list_index_type index, my_bool allow_negative_index)
{
    assert(DL);
    if (cyclic_dlinked_list_is_empty(DL))
    {
        fprintf(stderr, "Error, empty cyclic_dlinked_list, nothing to pop!\n");
    }
    if (allow_negative_index)
    {
        if (index < -(DL->size) || index >= DL->size)
        {
            fprintf(stderr, "Index out of range, index = %d, allowed: %d ~ %d\n", index, -(DL->size), DL->size - 1);
            return NULL;
        }
    }
    else
    {
        if (index < 0 || index >= DL->size)
        {

            fprintf(stderr, "Index out of range, index = %d, allowed: 0 ~ %d\n", index, DL->size - 1);
            return NULL;
        }
    }
    linked_list_index_type real_index = index > 0 ? index : DL->size + index;
    linked_list_index_type i = real_index < DL->size / 2 ? 0 : DL->size - 1;
    DNode *temp = real_index < DL->size / 2 ? DL->head : DL->tail;
    while (TRUE)
    {
        if (i == real_index)
        {
            void *ret = temp->data;
            if (temp == DL->head)
            {
                DL->head = temp->next;
            }
            if (temp == DL->tail)
            {
                DL->tail = temp->prev;
            }
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            DL->size--;
            return ret;
        }
        temp = real_index < DL->size / 2 ? temp->next : temp->prev;
        i = real_index < DL->size / 2 ? i + 1 : i - 1;
    }
}

extern void cyclic_dlinked_list_reverse(DLinkedList DL)
{
    assert(DL != NULL);
    DNode *temp = DL->head;
    DL->head = DL->tail;
    DL->tail = temp;
}

extern DLinkedList cyclic_dlinked_list_slice(DLinkedList DL, linked_list_index_type left, linked_list_index_type right, linked_list_size_type step)
{
    assert(DL != NULL);
    DLinkedList ret = cyclic_dlinked_list_new(0);
    if (cyclic_dlinked_list_is_empty(DL))
    {
        return ret;
    }
    linked_list_index_type real_left;
    linked_list_index_type real_right;
    linked_list_index_type i;
    if (step == 0)
    {
        fprintf(stderr, "step cannot be zero!\n");
        return NULL;
    }
    else if (step > 0)
    {
        if (left >= right)
        {
            return ret;
        }
        else
        {
            if (right <= -(DL->size))
            {
                return ret;
            }
            else if (right > -(DL->size) && right < 0)
            {
                real_right = DL->size + right;
            }
            else if (right >= 0 && right <= DL->size)
            {
                real_right = right;
            }
            else
            {
                real_right = DL->size;
            }

            if (left < -(DL->size))
            {
                linked_list_index_type real_left = 0;
            }
            else if (left >= -(DL->size) && left < 0)
            {
                real_left = DL->size + left;
            }
            else if (0 <= left && left <= DL->size - 1)
            {
                real_left = left;
            }
            else
            {
                return ret;
            }
            DNode *p_src = DL->head;
            my_bool start_flag = FALSE;
            i = 0;
            for (i = 0; i < real_left; ++i)
            {
                p_src = p_src->next;
            }
            for (i = 0; i < real_right - real_left; ++i)
            {
                if (i % step == 0)
                {
                    cyclic_dlinked_list_append_object(ret, p_src->data);
                }
                p_src = p_src->next;
            }
            return ret;
        }
    }
    else
    {
        if (left <= right)
        {
            return ret;
        }
        else
        {
            if (right <= -(DL->size + 1))
            {
                real_right = -1;
            }
            else if (right >= -(DL->size) && right < 0)
            {
                real_right = DL->size + right;
            }
            else if (right >= 0 && right < DL->size - 1)
            {
                real_right = right;
            }
            else /* if (right >= (DL->size - 1)) */
            {
                return ret;
            }

            if (left < -(DL->size))
            {
                return ret;
            }
            else if (left >= -(DL->size) || left < 0)
            {
                real_left = DL->size + left;
            }
            else if (left >= 0 && left < DL->size)
            {
                real_left = left;
            }
            else /* else if (left >= DL->size) */
            {
                real_left = DL->size - 1;
            }
            DNode *p_src = DL->tail;
            my_bool start_flag = FALSE;
            i = 0;
            for (i = DL->size - 1; i > real_right; --i)
            {
                p_src = p_src->prev;
            }
            for (i = 0; i < real_right - real_left; ++i)
            {
                if (i % step == 0)
                {
                    cyclic_dlinked_list_append_object(ret, p_src->data);
                }
                p_src = p_src->prev;
            }
            return ret;
        }
    }
}