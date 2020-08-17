#include "cyclic_linked_list_withtail.h"
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

extern my_bool linked_list_is_empty(const LinkedList L)
{
    if (L)
    {
        return FALSE;
    }
    return TRUE;
}

extern LinkedList linked_list_new(linked_list_size_type size)
{
    if (size < 0)
    {
        fprintf(stderr, "Invalid size: %d(expected size >= 0)\n", size);
        return NULL;
    }
    LinkedList p_tail = NULL;

    Node *head = NULL;
    Node *temp = NULL;
    linked_list_index_type i;
    for (i = 0; i < size; ++i)
    /*     while (size--) */
    { /*对于从无到有的过程是麻烦的*/
        if (i != 0)
        {
            temp = p_tail;
            head = p_tail->next;
            p_tail = node_new(NULL);
            temp->next = p_tail;
            p_tail->next = head;
        }
        else
        {
            p_tail = node_new(NULL);
            p_tail->next = p_tail;
        }
    }
    return p_tail;
}

extern LinkedList linked_list_new_from_args(linked_list_size_type size, ...)
{
    if (size <= 0)
    {
        fprintf(stderr, "Invalid size: %d(expected a positive value)\n", size);
        return NULL;
    };
    LinkedList p_tail = NULL;
    Node *p_temp = NULL;
    Node *p_head = NULL;
    va_list vl;
    va_start(vl, size);
    linked_list_index_type i;
    for (i = 0; i < size; ++i)
    {
        if (i == 0)
        {
            p_tail = node_new(va_arg(vl, void *));
            p_tail->next = p_tail;
        }
        else
        {
            p_temp = p_tail;
            p_head = p_tail->next;
            p_tail = node_new(va_arg(vl, void *));
            p_temp->next = p_tail;
            p_tail->next = p_head;
        }
    }
    va_end(vl);
    return p_tail;
}

extern LinkedList linked_list_new_from_elements(void **p, linked_list_size_type size)
{
    if (size < 0)
    {
        fprintf(stderr, "Invalid size: %d(expected size >= 0)\n", size);
        return NULL;
    }
    LinkedList p_tail = NULL;
    Node *p_temp = NULL;
    Node *p_head = NULL;
    linked_list_index_type i;
    for (i = 0; i < size; ++i)
    {
        if (i == 0)
        {
            p_tail = node_new(*(p + i));
            p_tail->next = p_tail;
        }
        else
        {
            p_temp = p_tail;
            p_head = p_tail->next;
            p_tail = node_new(*(p + i));
            p_temp->next = p_tail;
            p_tail->next = p_head;
        }
    }
    return p_tail;
}

extern void linked_list_print(const LinkedList L, const char *format, void (*elem_print)(FILE *, const void *))
{
    assert(L);
    if (strcmp(format, "MULTI_LINES") != 0 && strcmp(format, "SINGLE_LINE") != 0)
    {
        fprintf(stderr, "ERROR: Undefined format string\n");
    }

    Node *p_temp = L;
    while (p_temp)
    {
        p_temp = p_temp->next;
        elem_print(stdout, p_temp->data);
        if (strcmp(format, "MULTI_LINES") == 0)
        {
            if (p_temp == L)
            {
                fprintf(stdout, " -> back_to_start\n");
            }
            else
            {
                fprintf(stdout, " ->\n");
            }
        }
        if (strcmp(format, "SINGLE_LINE") == 0)
        {
            if (p_temp == L)
            {
                fprintf(stdout, " -> back_to_start\n");
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
    if (*Lp == NULL)
    {
        return;
    }
    Node *p_temp = (*Lp)->next;
    Node *p_temp_temp = NULL;
    while (p_temp != *Lp)
    {
        p_temp_temp = p_temp->next;
        p_temp->data = NULL;
        free(p_temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
        p_temp = p_temp_temp;
    }
    (*Lp)->data = NULL;
    free(*Lp);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *Lp = NULL;
}

extern linked_list_size_type linked_list_length(const LinkedList L)
{
    if (L == NULL)
    {
        return 0;
    }
    Node *p_temp = L->next;
    linked_list_size_type len;
    for (len = 1; p_temp != L; p_temp = p_temp->next)
    {
        ++len;
    }
    return len;
}

extern void *linked_list_right_delete_objects(LinkedList *Lp, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *))
{ /*从右边开始，删除n个和elem相同的Node，返回最后一个被删除的元素，空间o(1)*/
    assert(Lp);
    assert((*Lp));
    void *ret = NULL;
    if (n <= 0)
    {
        fprintf(stderr, "Invalid number n: %d (expected n>0)\n", n);
        return ret;
    }
    Node *p_temp = (*Lp)->next;
    my_bool current_first_flag = TRUE;
    linked_list_size_type count = 0;
    while (p_temp != (*Lp)->next ^ current_first_flag)
    { /*先找到n个和elem相同的*/
        if (elem_cmp(p_temp->data, elem) == 0)
        {
            count++;
        }
        p_temp = p_temp->next;
        current_first_flag = FALSE;
    }
    /* 重新开始删除 */
    p_temp = (*Lp)->next;
    Node *p_prev = *Lp;
    current_first_flag = TRUE;
    while (p_temp != (*Lp)->next ^ current_first_flag)
    {

        if (elem_cmp(p_temp->data, elem) == 0)
        {
            if (count-- <= n)
            {
                ret = p_temp->data;
                p_prev->next = p_temp->next;
                Node *p_free = p_temp;
                p_temp = p_temp->next;
                current_first_flag = FALSE;
                p_free->next = NULL;
                p_free->data = NULL;
                free(p_free);
#ifdef DEBUG_ALLOC_FREE_COUNT
                g_free_count++;
#endif
                p_free = NULL;
                if (count == 0)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
        p_prev = p_temp;
        p_temp = p_temp->next;
        current_first_flag = FALSE;
    }
    return ret;
}

extern void *linked_list_left_delete_objects(LinkedList *Lp, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *))
{ /*从左边开始，删除n个和elem相同的节点， 返回最后一个被删除的元素*/
    assert(Lp);
    assert((*Lp));
    void *ret = NULL;
    if (n <= 0)
    {
        fprintf(stderr, "Invalid number n: %d (expected n>0)\n", n);
        return ret;
    }
    Node *p_temp = (*Lp)->next;
    Node *p_prev = *Lp;
    my_bool current_first_flag = TRUE;
    linked_list_size_type count = 0;
    while (p_temp != (*Lp)->next ^ current_first_flag)
    {

        if (elem_cmp(p_temp->data, elem) == 0)
        {
            if (count++ < n)
            {
                ret = p_temp->data;
                p_prev->next = p_temp->next;
                Node *p_free = p_temp;
                p_temp = p_temp->next;
                current_first_flag = FALSE;
                p_free->next = NULL;
                p_free->data = NULL;
                free(p_free);
#ifdef DEBUG_ALLOC_FREE_COUNT
                g_free_count++;
#endif
                p_free = NULL;
                if (count == 0)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
        p_prev = p_temp;
        p_temp = p_temp->next;
        current_first_flag = FALSE;
    }
    return ret;
}

extern linked_list_index_type linked_list_right_search(const LinkedList L, const void *elem, int (*elem_cmp)(const void *, const void *))
{ /* 右查找，存在返回索引（索引从左边开始，头节点不算索引，第一个索引是0）, 不存在返回-1*/
    assert(L);
    linked_list_index_type i = 0;
    linked_list_index_type ret = -1;
    Node *p_temp = L->next;
    Node *p_prev = L;
    my_bool current_first_flag = TRUE;
    while (p_temp != L->next ^ current_first_flag)
    {
        if (elem_cmp(p_temp->data, elem) == 0)
        {
            {
                ret = i;
            }
            p_temp = p_temp->next;
            current_first_flag = FALSE;
            ++i;
        }
    }
    return ret;
}

extern linked_list_index_type linked_list_left_search(const LinkedList L, const void *elem, int (*elem_cmp)(const void *, const void *))
{ /* 左查找，存在返回索引（索引从左边开始，头节点不算索引，第一个索引是0）, 不存在返回-1*/
    assert(L);
    linked_list_index_type i = 0;
    linked_list_index_type ret = -1;
    Node *p_temp = L->next;
    Node *p_prev = L;
    my_bool current_first_flag = TRUE;
    while (p_temp != L->next ^ current_first_flag)
    {
        if (elem_cmp(p_temp->data, elem) == 0)
        {
            {
                ret = i;
                return ret;
            }
            p_temp = p_temp->next;
            current_first_flag = FALSE;
            ++i;
        }
    }
    return ret;
}

extern my_bool linked_list_contains(const LinkedList L, void *elem, int (*elem_cmp)(const void *, const void *))
{
    assert(L);
    Node *p_temp = L->next;
    Node *p_prev = L;
    my_bool current_first_flag = TRUE;
    while (p_temp != L->next ^ current_first_flag)
    {
        if (elem_cmp(p_temp->data, elem) == 0)
        {
            {
                return TRUE;
            }
            p_temp = p_temp->next;
            current_first_flag = FALSE;
        }
    }
    return FALSE;
}

extern void linked_list_append_object(LinkedList *Lp, void *elem)
{
    assert(Lp);
    if ((*Lp) == NULL)
    {
        (*Lp) = node_new(elem);
    }
    else
    {
        Node *temp = node_new(elem);
        Node *head = (*Lp)->next;
        (*Lp)->next = temp;
        temp->next = head;
        (*Lp) = temp;
    }
}

extern void linked_list_join(LinkedList *Lp_left, LinkedList *Lp_right)
{
    assert(Lp_left);
    assert(Lp_right);
    if (*Lp_left == NULL || *Lp_right)
    {
        return;
    }
    Node *p_left_head = (*Lp_left)->next;
    (*Lp_left)->next = (*Lp_right)->next;
    (*Lp_right)->next = p_left_head;
    *Lp_left = *Lp_right;
}

extern void *linked_list_pop(LinkedList *Lp)
{
    assert(Lp);
    Node *p_temp = *Lp;
    while (p_temp && p_temp->next != *Lp)
    {
        p_temp = p_temp->next;
    }
    p_temp->next = (*Lp)->next;
    void *ret = (*Lp)->data;
    (*Lp)->data = NULL;
    free(*Lp);
    *Lp = NULL;
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    if (p_temp != NULL)
    {
        *Lp = p_temp->next;
    }
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
    Node *p_temp = L->next;
    my_bool current_first_flag = TRUE;
    linked_list_index_type i = -1;
    while (p_temp != L->next ^ current_first_flag)
    {
        i++;
        if (i == index)
        {
            return p_temp->data;
        }
        p_temp = p_temp->next;
        current_first_flag = FALSE;
    }
    fprintf(stderr, "index out of range!\n");
    return NULL;
}

extern void linked_list_insert_object(LinkedList *Lp, linked_list_index_type index, void *data)
{
    assert(Lp);
    assert(*Lp);
    Node *p = node_new(data);
    status ret_status = linked_list_insert_node(Lp, index, p);
    if (ret_status == -1)
    {
        free(p);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
    }
}

extern void linked_list_insert_linked_list(LinkedList *Lp_src, linked_list_index_type index, LinkedList *Lp_new)
{ /*如果新L非空, 插入之后,新L将只剩下一个头节点*/
    assert(Lp_src);
    assert(Lp_new);
    if (*Lp_src == NULL)
    {
        fprintf(stderr, "source LinkedList is NULL!");
    }
    if (*Lp_new == NULL)
    {
        return;
    }
    if (index < 0)
    {
        fprintf(stderr, "Invalid index: %d(expected index >=0)\n", index);
    }
    Node *p_temp = (*Lp_src)->next;
    my_bool current_first_flag = TRUE;
    linked_list_index_type i = -1;
    while (p_temp != (*Lp_src) ^ current_first_flag)
    {
        i++;
        if (i == index)
        {
            Node *p_temp_next = p_temp->next;
            p_temp->next = (*Lp_new)->next;
            (*Lp_new)->next = p_temp_next;
            return;
        }
        p_temp = p_temp->next;
        current_first_flag = FALSE;
    }
    if (i < index)
    {
        fprintf(stderr, "Index out of boundary %d > %d\n", index, i);
        return;
    }
}

static void node_insert(Node *p_node_left, Node *p_node_new, Node *p_node_right)
{ /*必须确认插入新节点不会对其他数据造成影响, 比如要插入的新节点原来后面还有很多节点时,会导致内存泄漏*/
    p_node_left->next = p_node_new;
    p_node_new->next = p_node_right;
}

extern status linked_list_insert_node(LinkedList *Lp, linked_list_index_type index, Node *p_new_node)
{ /*必须确认插入新节点不会对其他数据造成影响, 比如要插入的新节点原来后面还有很多节点时,会导致内存泄漏*/
    /*多次操作存在隐患,比如可能会打成一个圈. 应该改为深拷贝*/
    assert(Lp);
    assert(*Lp);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index: %d(expected index >=0)\n", index);
        return -1;
    }
    Node *p_temp = (*Lp)->next;
    my_bool current_first_flag = TRUE;
    linked_list_index_type i = -1;
    while (p_temp != (*Lp)->next ^ current_first_flag)
    {
        i++;
        if (i == index)
        {
            node_insert(p_temp, p_new_node, p_temp->next);
            return 1;
        }
        p_temp = p_temp->next;
        current_first_flag = FALSE;
    }
    return -1;
}

extern void linked_list_index_assign(LinkedList L, linked_list_index_type index, void *elem)
{
    assert(L);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index: %d(expected index >=0)\n", index);
        return;
    }
    Node *p_temp = L->next;
    my_bool current_first_flag = TRUE;
    linked_list_index_type i = -1;
    while (p_temp != L->next ^ current_first_flag)
    {
        i++;
        if (i == index)
        {
            p_temp->data = elem;
            return;
        }
        p_temp = p_temp->next;
        current_first_flag = FALSE;
    }
    if (i < index)
    {
        fprintf(stderr, "Index out of size: %d > %d\n", index, i);
        return;
    }
}

extern void *linked_list_pop_i(LinkedList *Lp, linked_list_index_type index)
{
    assert(Lp);
    assert(*Lp);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index(expected index >=0)\n");
        return NULL;
    }
    Node *p_temp = (*Lp)->next;
    Node *p_prev = (*Lp);
    my_bool current_first_flag = TRUE;
    linked_list_index_type i = -1;
    while (p_temp != (*Lp)->next ^ current_first_flag)
    {
        ++i;
        if (i == index)
        {
            void *ret = p_temp->data;
            p_prev->next = p_temp->next;
            free(p_temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            return ret;
        }
        p_prev = p_temp;
        p_temp = p_temp->next;
        current_first_flag = FALSE;
    }
    if (i < index)
    {
        fprintf(stderr, "Index out of size: %d > %d\n", index, i);
        return NULL;
    }
    return NULL;
}

extern void linked_list_reverse(LinkedList *Lp)
{
    /* 要求时间O(n), 空间O(1) */
    assert(Lp);
    assert(*Lp);
    Node *p_future_tail = (*Lp)->next;
    Node *p_move = (*Lp)->next;
    Node *p_temp = NULL;
    Node *p_now = NULL;
    my_bool current_first_flag = TRUE;
    while (p_move != p_future_tail ^ current_first_flag)
    {
        p_temp = p_move;
        p_move = p_move->next;
        current_first_flag = FALSE;
        p_temp->next = p_now;
        p_now = p_temp;
    }
    p_future_tail->next = p_now;
    *Lp = p_future_tail;
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
    Node *p_src = L->next;
    Node *p_new = L_new->next;
    linked_list_index_type i = 0;
    my_bool start_flag = FALSE;
    my_bool current_first_flag = TRUE;
    while (p_src != L->next ^ current_first_flag) /*p_src->next 是错误的*/
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
        current_first_flag = FALSE;
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