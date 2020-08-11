#include "linked_list_withhead.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static Node *_node_alloc(void)
{
    Node *p = (Node *)calloc(1, sizeof(Node));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(p);
    return p;
}

static Node *node_new(void *data)
{
    Node *p = _node_alloc();
    assert(p);
    p->data = data;
    p->next = NULL;
    return p;
}

extern LinkedList linked_list_new(linked_list_size_type size)
{
    if (size < 0)
    {
        fprintf(stderr, "Invalid size: %d(expected size >= 0)\n", size);
        return NULL;
    }
    LinkedList head_node_p = node_new(NULL);

    Node *p_temp = head_node_p;
    linked_list_index_type i;
    for (i = 0; i < size; ++i)
    /*     while (size--) */
    {
        p_temp->next = node_new(NULL);
        p_temp = p_temp->next;
    }
    return head_node_p;
}

extern LinkedList linked_list_new_from_args(linked_list_size_type size, ...)
{
    if (size <= 0)
    {
        fprintf(stderr, "Invalid size: %d(expected a positive value)\n", size);
        return NULL;
    };
    LinkedList head_node_p = node_new(NULL);

    Node *p_temp = head_node_p;
    va_list vl;
    va_start(vl, size);
    linked_list_index_type i;
    for (i = 0; i < size; ++i)
    {
        p_temp->next = node_new(va_arg(vl, void *));
        p_temp = p_temp->next;
    }
    va_end(vl);
    return head_node_p;
}

extern LinkedList linked_list_new_from_elements(void **p, linked_list_size_type size)
{
    if (size < 0)
    {
        fprintf(stderr, "Invalid size: %d(expected size >= 0)\n", size);
        return NULL;
    }
    LinkedList head_node_p = node_new(NULL);
    Node *p_temp = head_node_p;
    linked_list_index_type i;
    for (i = 0; i < size; ++i)
    {
        p_temp->next = node_new(*(p + i));
        p_temp = p_temp->next;
    }
    return head_node_p;
}

extern void linked_list_print(const LinkedList L, const char *format, void (*elem_print)(FILE *, const void *))
{
    assert(L);
    if (strcmp(format, "MULTI_LINES") != 0 && strcmp(format, "SINGLE_LINE") != 0)
    {
        fprintf(stderr, "ERROR: Undefined format string\n");
    }

    Node *p_temp = L;
    while (p_temp && p_temp->next)
    {
        p_temp = p_temp->next;
        elem_print(stdout, p_temp->data);
        if (strcmp(format, "MULTI_LINES") == 0)
        {
            if (p_temp->next == NULL)
            {
                fprintf(stdout, "\n");
            }
            else
            {
                fprintf(stdout, " ->\n");
            }
        }
        if (strcmp(format, "SINGLE_LINE") == 0)
        {
            if (p_temp->next == NULL)
            {
                fprintf(stdout, "\n");
            }
            else
            {
                fprintf(stdout, " -> ");
            }
        }
    }
}

extern void linked_list_free(LinkedList *Lp)
{
    assert(Lp);
    assert(*Lp);
    Node *p_last = *Lp;
    Node *p_second;
    while (p_last && p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
        if (p_second != *Lp)
        {
            p_second->data = NULL;
            free(p_second);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
        }
    }
    p_second = NULL;
    if (p_last != *Lp)
    {
        p_last->data = NULL;
        free(p_last);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
        p_last = NULL;
    }
    /*不应该free data */
    /*  free((*Lp)->data);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif */

    (*Lp)->data = NULL;
    free(*Lp);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *Lp = NULL;
}

extern linked_list_size_type linked_list_length(const LinkedList L)
{
    assert(L);
    Node *p_temp;
    linked_list_size_type len;
    p_temp = L->next;
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

extern void *linked_list_right_delete_objects(LinkedList L, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *))
{ /*从右边开始，删除n个和elem相同的Node，返回最后一个被删除的元素，空间o(1)*/
    assert(L);
    void *ret = NULL;
    if (n <= 0)
    {
        fprintf(stderr, "Invalid number n: %d (expected n>0)\n", n);
        return ret;
    }
    Node *p_last = L;
    Node *p_second = L;
    /*先找到n个和elem相同的*/
    linked_list_size_type count = 0;
    while (p_last && p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
        if (elem_cmp(p_last->data, elem) == 0)
        {
            ++count;
        }
    }
    /* 重新开始删除 */
    p_last = L;
    p_second = L;
    while (p_last && p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
        if (elem_cmp(p_last->data, elem) == 0)
        {
            if (count-- <= n)
            {
                ret = p_last->data;
                p_second->next = p_last->next;
                Node *p_free = p_last;
                p_last = p_last->next;
                /* free(p_free->data); */
                /*
#ifdef DEBUG_ALLOC_FREE_COUNT
                g_free_count++;
#endif */
                p_free->next = NULL;
                p_free->data = NULL;
                free(p_free);
#ifdef DEBUG_ALLOC_FREE_COUNT
                g_free_count++;
#endif
                p_free = NULL;
            }
            if (count == 0)
            {
                break;
            }
        }
    }
    return ret;
}

extern void *linked_list_left_delete_objects(LinkedList L, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *))
{ /*从左边开始，删除n个和elem相同的节点， 返回最后一个被删除的元素*/
    assert(L);
    void *ret = NULL;
    if (n <= 0)
    {
        fprintf(stderr, "Invalid number n: %d (expected n>0)\n", n);
        return ret;
    }
    Node *p_last = L;
    Node *p_second = L;
    linked_list_size_type count = 0;
    while (p_last && p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
        if (elem_cmp(p_last->data, elem) == 0)
        {
            ++count;
            ret = p_last->data;
            p_second->next = p_last->next;
            Node *p_free = p_last;
            p_last = p_last->next;
            /* free(p_free->data); */ /* 不应该free*/
                                      /* 
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif */
            p_free->data = NULL;
            p_free->next = NULL;
            free(p_free);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            p_free = NULL;
        }
        if (count == n)
        {
            return ret;
        }
    }
    return ret;
}

extern linked_list_index_type linked_list_right_search(const LinkedList L, const void *elem, int (*elem_cmp)(const void *, const void *))
{ /* 右查找，存在返回索引（索引从左边开始，头节点不算索引，第一个索引是0）, 不存在返回-1*/
    assert(L);
    Node *p_temp = L;
    linked_list_index_type i = -1;
    linked_list_index_type ret = i;
    while (p_temp && p_temp->next)
    {
        p_temp = p_temp->next;
        ++i;
        if (elem_cmp(p_temp->data, elem) == 0)
        {
            ret = i;
        }
    }
    return ret;
}

extern linked_list_index_type linked_list_left_search(const LinkedList L, const void *elem, int (*elem_cmp)(const void *, const void *))
{ /* 左查找，存在返回索引（索引从左边开始，头节点不算索引，第一个索引是0）, 不存在返回-1*/
    assert(L);
    Node *p_temp = L;
    linked_list_index_type i = -1;
    while (p_temp && p_temp->next)
    {
        p_temp = p_temp->next;
        ++i;
        if (elem_cmp(p_temp->data, elem) == 0)
        {
            return i;
        }
    }
    return -1;
}

extern my_bool linked_list_contains(const LinkedList L, void *elem, int (*elem_cmp)(const void *, const void *))
{
    assert(L);
    Node *p_temp = L;
    while (p_temp && p_temp->next)
    {
        p_temp = p_temp->next;
        if (elem_cmp(p_temp->data, elem) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

extern void linked_list_append_object(LinkedList L, void *elem)
{
    assert(L);
    Node *p_temp = L;
    while (p_temp && p_temp->next)
    {
        p_temp = p_temp->next;
    }
    p_temp->next = node_new(elem);
}

extern void linked_list_join(LinkedList L_left, LinkedList L_right)
{ /* 如果右边非空,连过来之后,右边链表还在，但是不能通过右的头节点访问，右边的节点就只剩头节点，
 这意味着以后只能用左边的头节点去访问了*/
    assert(L_left);
    assert(L_right);
    Node *p_temp = L_left;
    while (p_temp && p_temp->next)
    {
        p_temp = p_temp->next;
    }
    if (L_right->next)
    {
        p_temp->next = L_right->next;
        L_right->next = NULL;
        /* linked_list_free(L_right); */ /*不应该free*/
        /* L_right = NULL; */
    }
}

extern void *linked_list_pop(LinkedList L)
{
    assert(L);
    Node *p_last = L;
    Node *p_second;
    if (p_last->next == NULL)
    {
        return NULL;
    }
    while (p_last && p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
    }
    p_second->next = NULL;
    void *ret = p_last->data;
    free(p_last);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    p_last = NULL;
    return ret;
}

extern void *linked_list_object_at(const LinkedList L, linked_list_index_type index)
{
    assert(L);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index: %d(expected index >=0)\n", index);
        return NULL;
    }
    linked_list_index_type i = -1;
    Node *p_temp = L;
    while (p_temp && p_temp->next)
    {
        p_temp = p_temp->next;
        i++;
        if (i == index)
        {
            return p_temp->data;
        }
    }
    return NULL;
}

extern void linked_list_insert_object(LinkedList L, linked_list_index_type index, void *data)
{
    assert(L);
    Node *p = node_new(data);
    status ret_status = linked_list_insert_node(L, index, p);
    if (ret_status == -1)
    {
        free(p);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
    }
}

extern void linked_list_insert_linked_list(LinkedList L_src, linked_list_index_type index, LinkedList L_new)
{ /*如果新L非空, 插入之后,新L将只剩下一个头节点*/
    assert(L_src);
    assert(L_new);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index: %d(expected index >=0)\n", index);
    }
    if (L_new->next == NULL)
    {
        return;
    }
    Node *p_last = L_src;
    Node *p_second = L_src;
    linked_list_index_type i = -1;
    while (p_last && p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
        i++;
        if (i == index)
        {
            break;
        }
    }
    if (i < index)
    {
        fprintf(stderr, "Index out of boundary %d > %d\n", index, i);
        return;
    }
    if (p_last == L_src)
    {
        L_src->next = L_new->next;
    }
    else
    {
        p_second->next = L_new->next;
        Node *p_temp = L_new;
        while (p_temp && p_temp->next)
        {
            p_temp = p_temp->next;
        }
        p_temp->next = p_last;
    }
    L_new->next = NULL;
    /* linked_list_free(L_new);
    L_new = NULL; */
}

static void node_insert(Node *p_node_left, Node *p_node_new, Node *p_node_right)
{ /*必须确认插入新节点不会对其他数据造成影响, 比如要插入的新节点原来后面还有很多节点时,会导致内存泄漏*/
    p_node_left->next = p_node_new;
    p_node_new->next = p_node_right;
}

extern status linked_list_insert_node(LinkedList L, linked_list_index_type index, Node *p_new_node)
{ /*必须确认插入新节点不会对其他数据造成影响, 比如要插入的新节点原来后面还有很多节点时,会导致内存泄漏*/
    /*多次操作存在隐患,比如可能会打成一个圈. 应该改为深拷贝*/
    assert(L);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index: %d(expected index >=0)\n", index);
        return -1;
    }
    Node *p_last = L;
    Node *p_second;
    linked_list_index_type i = -1;
    while (p_last && p_last->next)
    {
        i++;
        p_second = p_last;
        p_last = p_last->next;
        if (i == index)
        {
            break;
        }
    }
    if (i < index)
    {
        fprintf(stderr, "Index out of size: %d > %d\n", index, i);
        return -1;
    }
    if (p_last == L)
    {
        L->next = p_new_node;
    }
    else
    {
        node_insert(p_second, p_new_node, p_last);
    }
    return 1;
}

extern void linked_list_index_assign(const LinkedList L, linked_list_index_type index, void *elem)
{
    assert(L);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index: %d(expected index >=0)\n", index);
        return;
    }
    Node *p_temp = L;
    linked_list_index_type i = -1;
    while (p_temp && p_temp->next)
    {
        i++;
        p_temp = p_temp->next;
        if (i == index)
        {
            p_temp->data = elem;
            return;
        }
    }
    if (i < index)
    {
        fprintf(stderr, "Index out of size: %d > %d\n", index, i);
        return;
    }
}

extern void *linked_list_pop_i(LinkedList L, linked_list_index_type index)
{
    assert(L);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index(expected index >=0)\n");
        return NULL;
    }
    Node *p_last = L;
    Node *p_second;
    linked_list_index_type i = -1;
    while (p_last && p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
        ++i;
        if (i == index)
        {
            p_second->next = p_last->next;
            void *ret = p_last->data;
            free(p_last);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            p_last = NULL;
            return ret;
        }
    }
    if (i < index)
    {
        fprintf(stderr, "Index out of size: %d > %d\n", index, i);
        return NULL;
    }
    return NULL;
}

extern void linked_list_reverse(LinkedList L)
{
    /* 要求时间O(n), 空间O(1) */
    Node *p_move = L->next;
    Node *p_temp;
    Node *p_now = NULL;
    while (p_move != NULL)
    {
        p_temp = p_move;
        p_move = p_move->next;
        p_temp->next = p_now;
        p_now = p_temp;
    }
    L->next = p_now;
    return;
    /* 如果要求返回一个L, 稍微改一下即可 */
}

extern LinkedList linked_list_slice(LinkedList L, linked_list_index_type left, linked_list_index_type right, linked_list_index_type step)
{ /* [,)  不支持负的索引和负的步长*/
    /*shallow copy, 虽然会出现一些新的node， 但是node里面的data是shallo copy， 直接拿的指针*/
    assert(L);
    if (left < 0 || right < 0)
    {
        fprintf(stderr, "Invalid index: left=%d, right=%d(expected index >=0)\n", left, right);
        return NULL;
    }
    if (step <= 0)
    {
        fprintf(stderr, "Invalid step: %d(expected step>0)\n", step);
        return NULL;
    }
    if (left > right)
    {
        fprintf(stderr, "Invalid index: left>=right(expected left<right)\n");
        return NULL;
    }
    linked_list_size_type src_len = linked_list_length(L);
    if (src_len <= 0)
    {
        fprintf(stderr, "Source list is empty\n");
        return NULL;
    }
    if (left > src_len - 1)
    {
        fprintf(stderr, "Invalid index: left:%d is out of boundary\n", left);
        return NULL;
    }

    linked_list_size_type min_len = right - left < src_len - left ? right - left : src_len - left;
    linked_list_size_type new_len = min_len % step == 0 ? min_len / step : min_len / step + 1;

    LinkedList L_new = linked_list_new(new_len);
    assert(L_new);
    if (left == right)
    {
        return L_new;
    }
    linked_list_size_type cumulative_step = step;
    Node *p_src = L;
    Node *p_new = L_new->next;
    linked_list_index_type i = -1;
    my_bool start_flag = FALSE;
    while (p_src) /*p_src->next 是错误的*/
    {
        if (i == left)
        {
            start_flag = TRUE;
        }
        if (start_flag)
        {

            if (cumulative_step == step)
            {
                assert(p_src);
                p_new->data = p_src->data; /*shallow copy*/
                p_new = p_new->next;
                cumulative_step = 0;
                /*用累计步长代替取余*/
            }
        }
        p_src = p_src->next;
        ++i;
        if (i == right)
        {
            break;
        }
        if (start_flag)
        {
            cumulative_step++;
        }
    }
    return L_new;
}