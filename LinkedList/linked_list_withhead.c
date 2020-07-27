/* linked_list.c */
#include "linked_list_withhead.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
Node *_node_alloc(size_type node_p_size)
{
    Node *p = (Node *)calloc(1, node_p_size);
    return p;
}
void *_node_free(Node *p)
{
}


LinkedList linked_list_new(size_type size, ObjectFuncs *o)
{
    if (size < 0)
    {
        fprintf(stderr, "Invalid size: %d(expected size >= 0)\n", size);
        return NULL;
    }
    LinkedList head_node_p = node_new(o);
    assert(head_node_p);

    Node *p_temp = head_node_p;
    int i;
    for (i = 0; i < size; ++i)
    /*     while (size--) */
    {
        void *p_obj = ((ObjectFuncs *)(head_node_p->data))->p_func_object_alloc(((ObjectFuncs *)(head_node_p->data))->object_pointer_size);
        p_temp->next = node_new(p_obj);
        assert(p_temp);
        p_temp = p_temp->next;
    }
    return head_node_p;
}

LinkedList linked_list_new_from_args(ObjectFuncs *o, int size, ...)
{
    if (size <= 0)
    {
        fprintf(stderr, "Invalid size: %d(expected a positive value)\n", size);
        return NULL;
    };
    LinkedList head_node_p = node_new(o);
    assert(head_node_p);

    Node *p_temp = head_node_p;

    va_list vl;
    va_start(vl, size);
    int i;
    for (i = 0; i < size; ++i)
    {
        p_temp->next = node_new(va_arg(vl, void *));
        assert(p_temp);
        p_temp = p_temp->next;
    }
    va_end(vl);
    return head_node_p;
}

LinkedList linked_list_new_from_objects(void **p, size_type size, ObjectFuncs *o)
{
    if (size < 0)
    {
        fprintf(stderr, "Invalid size: %d(expected size >= 0)\n", size);
        return NULL;
    }
    LinkedList head_node_p = node_new(o);
    assert(head_node_p);
    Node *p_temp = head_node_p;
    int i;
    for (i = 0; i < size; ++i)
    {
        p_temp->next = node_new(*(p + i));
        assert(p_temp);
        p_temp = p_temp->next;
    }
    return head_node_p;
}

void linked_list_print(const LinkedList L, const char *format)
{
    assert(L);
    if (strcmp(format, "MULTI_LINES") != 0 && strcmp(format, "SINGLE_LINE") != 0)
    {
        fprintf(stderr, "ERROR: Undefined format string\n");
    }

    Node *p_temp = L;
    while (p_temp->next)
    {
        p_temp = p_temp->next;
        ((ObjectFuncs *)(L->data))->p_func_object_print(stdout, p_temp->data);
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

void linked_list_free(LinkedList L)
{
    assert(L);
    Node *p_last = L;
    Node *p_second;
    while (p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
        if (p_second != L)
        {
            ((ObjectFuncs *)(L->data))->p_func_object_free(p_second->data);
            p_second->data = NULL;
            free(p_second);
        }
    }
    p_second = NULL;
    if (p_last != L)
    {

        ((ObjectFuncs *)(L->data))->p_func_object_free(p_last->data);
        p_last->data = NULL;
        free(p_last);
        p_last = NULL;
    }
    free(L->data);
    L->data = NULL;
    free(L);
    L = NULL; /*no use*/
}

size_type linked_list_length(const LinkedList L)
{
    assert(L);
    Node *p_temp;
    size_type len;
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
void *linked_list_right_delete_objects(LinkedList L, const void *obj, size_type n)
{ /*空间o(1)*/
    assert(L);
    void *ret = NULL;
    if (n <= 0)
    {
        fprintf(stderr, "Invalid number n: %d (expected n>0)\n", n);
        return ret;
    }
    Node *p_last = L;
    Node *p_second = L;
    size_type count = 0;
    while (p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
        if (((ObjectFuncs *)(L->data))->p_func_object_compare(p_last->data, obj) == 0)
        {
            ++count;
        }
    }
    p_last = L;
    p_second = L;
    while (p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
        if (((ObjectFuncs *)(L->data))->p_func_object_compare(p_last->data, obj) == 0)
        {
            if (count-- <= n)
            {
                ret = p_last->data;
                p_second->next = p_last->next;
                Node *p_free = p_last;
                p_last = p_last->next;
                free(p_free->data);
                p_free->data = NULL;
                free(p_free);
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
void *linked_list_left_delete_objects(LinkedList L, const void *obj, size_type n)
{
    assert(L);
    void *ret = NULL;
    if (n <= 0)
    {
        fprintf(stderr, "Invalid number n: %d (expected n>0)\n", n);
        return ret;
    }
    Node *p_last = L;
    Node *p_second = L;
    size_type count = 0;
    while (p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
        if (((ObjectFuncs *)(L->data))->p_func_object_compare(p_last->data, obj) == 0)
        {
            ++count;
            ret = p_last->data;
            p_second->next = p_last->next;
            Node *p_free = p_last;
            p_last = p_last->next;
            free(p_free->data);
            p_free->data = NULL;
            free(p_free);
            p_free = NULL;
        }
        if (count == n)
        {
            return ret;
        }
    }
    return ret;
}
size_type linked_list_right_search(const LinkedList L, const void *obj)
{
    assert(L);
    Node *p_temp = L;
    size_type i = -1;
    size_type ret = ERROR_SIZE;
    while (p_temp->next)
    {
        p_temp = p_temp->next;
        ++i;
        if (((ObjectFuncs *)(L->data))->p_func_object_compare(p_temp->data, obj) == 0)
        {
            ret = i;
        }
    }
    return ret;
}
size_type linked_list_left_search(const LinkedList L, const void *obj)
{
    assert(L);
    Node *p_temp = L;
    size_type i = -1;
    while (p_temp->next)
    {
        p_temp = p_temp->next;
        ++i;
        if (((ObjectFuncs *)(L->data))->p_func_object_compare(p_temp->data, obj) == 0)
        {
            return i;
        }
    }
    return ERROR_SIZE;
}
my_bool linked_list_contains(const LinkedList L, void *obj)
{
    assert(L);
    Node *p_temp = L;
    while (p_temp->next)
    {
        p_temp = p_temp->next;
        if (((ObjectFuncs *)(L->data))->p_func_object_compare(p_temp->data, obj) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}
void linked_list_append_object(LinkedList L, void *obj)
{
    assert(L);
    Node *p_temp = L;
    while (p_temp->next)
    {
        p_temp = p_temp->next;
    }
    p_temp->next = node_new(obj);
}

void linked_list_join(LinkedList L_left, LinkedList L_right)
{ /* 如果右边非空,连过来之后,就被消除了, 相当于吃掉*/
    assert(L_left);
    assert(L_right);
    Node *p_temp = L_left;
    while (p_temp->next)
    {
        p_temp = p_temp->next;
    }
    if (L_right->next)
    {
        p_temp->next = L_right->next;
        L_right->next = NULL;
        linked_list_free(L_right);
        L_right = NULL;
    }
}

void *linked_list_pop(LinkedList L)
{
    assert(L);
    Node *p_last = L;
    Node *p_second;
    if (p_last->next == NULL)
    {
        return NULL;
    }
    while (p_last->next)
    {
        p_second = p_last;
        p_last = p_last->next;
    }
    p_second->next = NULL;
    void *ret = p_last->data;
    free(p_last);
    p_last = NULL;
    return ret;
}

void *linked_list_object_at(const LinkedList L, size_type index)
{
    assert(L);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index: %d(expected index >=0)\n", index);
        return NULL;
    }
    size_type i = -1;
    Node *p_temp = L;
    while (p_temp->next)
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

Node *node_new(void *object)
{
    Node *p = _node_alloc(sizeof(Node));
    assert(p);
    p->data = object;
    p->next = NULL;
    return p;
}
void linked_list_insert_object(LinkedList L, size_type index, void *data)
{
    assert(L);
    Node *p = node_new(data);
    linked_list_insert_node(L, index, p);
}
void linked_list_insert_linked_list(LinkedList L_src, size_type index, LinkedList L_new)
{ /*如果新L非空, 插入之后,新L的头节点将被删除释放*/
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
    size_type i = -1;
    while (p_last->next)
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
        while (p_temp->next)
        {
            p_temp = p_temp->next;
        }
        p_temp->next = p_last;
    }
    L_new->next = NULL;
    linked_list_free(L_new);
    L_new = NULL;
}
void linked_list_insert_node(LinkedList L, size_type index, Node *p_new_node)
{ /*必须确认插入新节点不会对其他数据造成影响, 比如要插入的新节点原来后面还有很多节点时,会导致内存泄漏*/
    /*多次操作存在隐患,比如可能会打成一个圈. 应该改为深拷贝*/

    assert(L);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index: %d(expected index >=0)\n", index);
        return;
    }
    Node *p_last = L;
    Node *p_second;
    size_type i = -1;
    while (p_last->next != NULL)
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
        return;
    }
    if (p_last == L)
    {
        L->next = p_new_node;
    }
    else
    {
        node_insert(p_second, p_new_node, p_last);
    }
}
void node_insert(Node *p_node_left, Node *p_node_new, Node *p_node_right)
{ /*必须确认插入新节点不会对其他数据造成影响, 比如要插入的新节点原来后面还有很多节点时,会导致内存泄漏*/

    p_node_left->next = p_node_new;
    p_node_new->next = p_node_right;
}
void linked_list_index_assign(const LinkedList L, size_type index, void *object)
{
    assert(L);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index: %d(expected index >=0)\n", index);
        return;
    }
    Node *p_temp = L;
    size_type i = -1;
    while (p_temp->next != NULL)
    {
        i++;
        p_temp = p_temp->next;
        if (i == index)
        {
            p_temp->data = object;
            return;
        }
    }
    if (i < index)
    {
        fprintf(stderr, "Index out of size: %d > %d\n", index, i);
        return;
    }
}
void *linked_list_pop_i(LinkedList L, size_type index)
{
    assert(L);
    if (index < 0)
    {
        fprintf(stderr, "Invalid index(expected index >=0)\n");
        return NULL;
    }
    Node *p_last = L;
    Node *p_second;
    size_type i = -1;
    while (p_last->next != NULL)
    {
        p_second = p_last;
        p_last = p_last->next;
        ++i;
        if (i == index)
        {
            p_second->next = p_last->next;
            void *ret = p_last->data;
            free(p_last);
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

void linked_list_reverse(LinkedList L)
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

LinkedList linked_list_slice(LinkedList L, size_type left, size_type right, size_type step)
{ /* [,) */
    /*shallow copy*/
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
    size_type src_len = linked_list_length(L);
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

    size_type min_len = right - left < src_len - left ? right - left : src_len - left;
    size_type new_len = min_len % step == 0 ? min_len / step : min_len / step + 1;

    LinkedList L_new = linked_list_new(new_len, L->data);
    assert(L_new);
    if (left == right)
    {
        return L_new;
    }
    size_type cumulative_step = step;
    Node *p_src = L;
    Node *p_new = L_new->next;
    size_type i = -1;
    my_bool start_flag = FALSE;
    while (p_src->next)
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