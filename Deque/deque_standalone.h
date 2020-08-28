/* deque.h
Usage:
****************************************
#define T         the_type_you_want
#define NULL_ELEM null_value_of_the_type
#include "path_to_this_h_file"
****************************************
For example:
****************************************
typedef struct Student
{
    const char *name;
    unsigned int age;
} Student;
#define T Student
#define NULL_ELEM \
    (Student) { NULL, 0 }
#include "deque_standalone.h"
****************************************
#define T void*
#define NULL_ELEM NULL
#include "deque_standalone.h"
****************************************
Note: Only use it once in each .c file!
*/
#include "../my_bool.h"
#include "../my_debug.h"

#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stddef.h>
/*
#ifndef _DEQUE_H_
#define _DEQUE_H_
*/
#ifndef _DEQUE_SIZE_TYPE_
#define _DEQUE_SIZE_TYPE_
typedef int32_t deque_size_type; /* 负值是有需要的 */
#endif

typedef struct DequeIterator
{
    /* Note that there are positional difference between two types of iters: head_iter and tail_iter. */
    T *cur;
    T *first;
    T *last;
    T **node; /* "buffer" and "node" are synonymous */
    deque_size_type num_elems_each_buffer;
} DequeIterator;

typedef enum deque_iter_type
{
    head = 0,
    tail = 1,
} deque_iter_type;

typedef struct Deque
{
    T **map_pointer;
    deque_size_type num_elems_each_buffer;
    deque_size_type map_size;
    DequeIterator *head_iter_p;
    DequeIterator *tail_iter_p;
} Deque;

/* functions declaration */
/*      assumed public functions are as follows */

/*      end of public functions */
/*********************************************************************************************************/
/********************* Above is for brief use. Functions source code is as follows ***********************/
/*********************************************************************************************************/
/*      assumed private functions are as follows, which normally needn't to be used directly */

/*      end of private functions */
/* end of functions declaration */

static const deque_size_type default_inital_map_size = 8;
static const deque_size_type default_num_elems_each_buffer = 8;

static deque_size_type _revise_num_elems_each_buffer(deque_size_type input_num_elems_each_buffer)
{
    return input_num_elems_each_buffer > default_num_elems_each_buffer ? input_num_elems_each_buffer : default_num_elems_each_buffer;
}

static void _deque_iter_set_node(DequeIterator *di_p, deque_iter_type di_t, T **new_node_addr, deque_size_type num_elems_each_buffer)
{
    di_p->node = new_node_addr;
    di_p->num_elems_each_buffer = num_elems_each_buffer;
    if (di_t == head)
    {
        di_p->first = *new_node_addr;
        di_p->last = di_p->first + (ptrdiff_t)(num_elems_each_buffer - 1); /* 最后元素的首地址*/
    }
    else if (di_t == tail)
    {
        di_p->first = (*new_node_addr) + 1;
        di_p->last = di_p->first + (ptrdiff_t)(num_elems_each_buffer); /* 最后元素的末尾 */
    }
    else
    {
        fprintf(stderr, "ERROR: Undefined deque_iter_type!\n");
        exit(-1);
    }
}

static void _deque_iter_add(DequeIterator *di_p, deque_iter_type di_t, deque_size_type n)
{
    ptrdiff_t offset = n + (di_p->cur - di_p->first);
    if (offset >= 0 && offset < (ptrdiff_t)(di_p->num_elems_each_buffer))
    { /*目标位置在统一缓冲区*/
        di_p->cur += n;
    }
    else
    {
        ptrdiff_t node_offset = offset > 0 ? offset / (ptrdiff_t)(di_p->num_elems_each_buffer) : -(ptrdiff_t)((-offset - 1) / di_p->num_elems_each_buffer) - 1;
        _deque_iter_set_node(di_p, di_t, di_p->node + node_offset, di_p->num_elems_each_buffer);     /*切换至正确的节点*/
        di_p->cur = di_p->first + (offset - node_offset * (ptrdiff_t)(di_p->num_elems_each_buffer)); /*切换至正确的元素*/
    }
}

static DequeIterator *_deque_iter_alloc(void)
{
    DequeIterator *ret = (DequeIterator *)malloc(sizeof(DequeIterator));
    assert(ret);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    return ret;
}

static T *_node_alloc(deque_size_type num_elems_each_buffer)
{
    T *ret = (T *)calloc(num_elems_each_buffer, sizeof(T));
    assert(ret);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    return ret;
}

static T **_map_alloc(deque_size_type map_size)
{
    assert(map_size > 0);
    T **ret = (T **)calloc(map_size, sizeof(T *));
    assert(ret);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    return ret;
}

static Deque *deque_init(deque_size_type num_elems, deque_size_type num_elems_each_buffer)
{ /*set num_elems_each_buffer <= 0 will get a default value 8 */

    /* 初始计算 */
    assert(num_elems >= 0);
    num_elems_each_buffer = _revise_num_elems_each_buffer(num_elems_each_buffer);
    deque_size_type num_nodes = num_elems / num_elems_each_buffer + 1;
    deque_size_type map_size = default_inital_map_size > num_nodes + 2 ? default_inital_map_size : num_nodes + 2;

    /* 开辟Deque空间 */
    Deque *dq_p = (Deque *)malloc(sizeof(Deque));
    assert(dq_p);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    dq_p->num_elems_each_buffer = num_elems_each_buffer;
    dq_p->map_size = map_size;
    dq_p->map_pointer = _map_alloc(map_size);
    assert(dq_p->map_pointer);

    /* 为map中的每个节点分配空间 */
    /* deque_size_type i;
    for (i = 0; i < map_size; ++i)
    {
        *(dq_p->map_pointer + i) = (T *)calloc(num_elems_each_buffer, sizeof(T));
        assert(*(dq_p->map_pointer + i));
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_alloc_count++;
#endif
    } */

    /* C++STL写法大致如下, 这是让head和tail尽量靠中间的写法 */
    T **map_ptr_head = dq_p->map_pointer + (map_size - num_nodes) / 2;
    T **map_ptr_tail = map_ptr_head + num_nodes - 1;

    /* 对从head到tail的每个节点分配空间 */
    T **map_ptr_temp;
    for (map_ptr_temp = map_ptr_head; map_ptr_temp <= map_ptr_tail; ++map_ptr_temp)
    {
        *map_ptr_temp = _node_alloc(num_elems_each_buffer);
        assert(*map_ptr_temp);
    }

    /* 创建iter */
    dq_p->head_iter_p = _deque_iter_alloc();
    assert(dq_p->head_iter_p);
    dq_p->tail_iter_p = _deque_iter_alloc();
    assert(dq_p->tail_iter_p);

    /* 设置iter */
    _deque_iter_set_node(dq_p->head_iter_p, head, map_ptr_head, num_elems_each_buffer);
    _deque_iter_set_node(dq_p->tail_iter_p, tail, map_ptr_tail, num_elems_each_buffer);
    dq_p->head_iter_p->cur = dq_p->head_iter_p->first;
    dq_p->tail_iter_p->cur = num_elems % num_elems_each_buffer == 0 ? (num_elems == 0 ? dq_p->tail_iter_p->first : dq_p->tail_iter_p->last) : (dq_p->tail_iter_p->first + num_elems % num_elems_each_buffer - 1); /* cur指向元素的末尾（而不是地址） */

    return dq_p;
}

static void deque_free(Deque **dq_pp)
{
    assert(dq_pp != NULL);
    assert((*dq_pp) != NULL);
    T **map_ptr_temp;
    for (map_ptr_temp = (*dq_pp)->head_iter_p->node; map_ptr_temp <= (*dq_pp)->tail_iter_p->node; ++map_ptr_temp)
    {
        free(*map_ptr_temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
    }
    free((*dq_pp)->head_iter_p);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free((*dq_pp)->tail_iter_p);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free((*dq_pp)->map_pointer);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(*dq_pp);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *dq_pp = NULL;
}

static void deque_clear(Deque *dq_p)
{ /* 清空元素, 保留一个buffer, 也就是成为空deque */
    assert(dq_p != NULL);
    T **temp_node;
    for (temp_node = dq_p->head_iter_p->node + 1; temp_node < dq_p->tail_iter_p->node; ++temp_node)
    { /* 删除除了头尾之外的 */
        memset(temp_node, 0, dq_p->num_elems_each_buffer * sizeof(T));
        free(temp_node);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
    }
    if (dq_p->head_iter_p->node != dq_p->tail_iter_p->node)
    { /* 头尾是两个不同的 node */
        memset(dq_p->head_iter_p->node, 0, dq_p->num_elems_each_buffer * sizeof(T));
        memset(dq_p->tail_iter_p->node, 0, dq_p->num_elems_each_buffer * sizeof(T));
        free(dq_p->tail_iter_p->node);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
    }
    else
    { /* 头尾都是一个 */
        memset(dq_p->head_iter_p->node, 0, dq_p->num_elems_each_buffer * sizeof(T));
    }
    dq_p->head_iter_p->cur = dq_p->head_iter_p->first;
    dq_p->tail_iter_p->cur = dq_p->tail_iter_p->first;
}

static void deque_print(const Deque *dq_p, FILE *fp, void (*elem_print)(FILE *, const T *))
{
}
static deque_size_type deque_elem_count(const Deque *dq_p)
{
    assert(dq_p != NULL);
    T **temp_node;
    deque_size_type num_used_nodes = dq_p->tail_iter_p->node - dq_p->head_iter_p->node + 1;
    if (num_used_nodes == 1)
    {
        return dq_p->tail_iter_p->cur - dq_p->head_iter_p->cur;
    }
    else
    {
        return dq_p->tail_iter_p->cur - dq_p->tail_iter_p->first + dq_p->head_iter_p->last - dq_p->head_iter_p->cur + dq_p->num_elems_each_buffer * (num_used_nodes - 2);
    }
}

static T deque_get_elem_at_i(const Deque *dq_p, const deque_size_type index)
{ /*如果想用这个函数的返回值进行赋值操作，实现如a[i]=2的功能，请确认T是可修改的左值，毕竟C语言不支持引用&。
    For assignment operations using this func, such as "a[i]=2", T is supposed to be modifiable lvalue.
    Calling by reference is not supported in C (not C++), after all. */
    /* python like negative index is allowed */
    if (index >= 0)
    {
        if (index > deque_elem_count(dq_p) - 1)
        {
            fprintf(stderr, "ERROR: index out of range!\n");
            return NULL_ELEM;
        }
        if (index <= dq_p->head_iter_p->last - dq_p->head_iter_p->cur)
        {
            return *(dq_p->head_iter_p->cur + index);
        }
        else
        {
            ptrdiff_t node_increment = (index - (dq_p->head_iter_p->last - dq_p->head_iter_p->cur)) / (dq_p->num_elems_each_buffer) + 1;

            ptrdiff_t last_node_mod = (index - (dq_p->head_iter_p->last - dq_p->head_iter_p->cur)) % (dq_p->num_elems_each_buffer);
            if (last_node_mod == 0)
            {
                return *(*(dq_p->head_iter_p->node + node_increment - 1) + dq_p->num_elems_each_buffer - 1);
            }
            else
            {
                return *(*(dq_p->head_iter_p->node + node_increment) + last_node_mod - 1);
            }
        }
    }
    else
    {
        if (index < -(deque_elem_count(dq_p)))
        {
            fprintf(stderr, "ERROR: index out of range\n");
            return NULL_ELEM;
        }
        if (index >= dq_p->tail_iter_p->first - dq_p->tail_iter_p->cur - 1)
        {
            return *(dq_p->head_iter_p->cur + index);
            /* 这里必须返回元素地址, 而不是末尾*/
        }
        else
        {
            ptrdiff_t node_decrement = (index - (dq_p->tail_iter_p->first - dq_p->tail_iter_p->cur - 1)) / (dq_p->num_elems_each_buffer) - 1; /* negative */

            ptrdiff_t first_node_mod = (index - (dq_p->tail_iter_p->first - dq_p->tail_iter_p->cur - 1)) % (dq_p->num_elems_each_buffer);
            if (first_node_mod == 0)
            {
                return *(*(dq_p->tail_iter_p->node + node_decrement) + 0);
            }
            else
            {
                return *(*(dq_p->head_iter_p->node + node_decrement) + dq_p->num_elems_each_buffer + first_node_mod);
            }
        }
    }
}

static T deque_get_first_index_of_elem(const Deque *dq_p, const T elem, my_bool (*elem_cmp)(const T, const T))
{
}

static my_bool deque_is_empty(const Deque *dq_p)
{
    assert(dq_p != NULL);
    if (dq_p->head_iter_p->node == dq_p->tail_iter_p->node && dq_p->head_iter_p->cur == dq_p->tail_iter_p->cur)
    {
        return TRUE;
    }
    return FALSE;
}

static T deque_pop_head(Deque *dq_p)
{
    assert(dq_p != NULL);
    if (deque_is_empty(dq_p)) /* 空deque判断 */
    {
        fprintf(stderr, "ERROR: Empty deque!\n");
        return NULL_ELEM;
    }
    if (dq_p->head_iter_p->cur != dq_p->head_iter_p->last || dq_p->head_iter_p->node == dq_p->head_iter_p->node)
    { /* head对应buffer至少有两个元素, 或者 整个deque只有一个buffer时, 也就是deque只有一个元素的情况, pop后deque即将变成空的 */
        T ret = *(dq_p->head_iter_p->cur);
        _deque_iter_add(dq_p->head_iter_p, head, 1);
        return ret;
    }
    else
    { /* head对应buffer只有一个元素,且和head不在一个buffer */
        T ret = *(dq_p->head_iter_p->cur);
        free(dq_p->head_iter_p->node);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
        _deque_iter_set_node(dq_p->head_iter_p, head, dq_p->head_iter_p->node + 1, dq_p->num_elems_each_buffer);
        /*         dq_p->head_iter_p->node - 1 = NULL; */
        dq_p->head_iter_p->cur = dq_p->head_iter_p->first;
        return ret;
    }
}

static T deque_pop_tail(Deque *dq_p)
{
    assert(dq_p != NULL);
    if (deque_is_empty(dq_p)) /* 空deque判断 */
    {
        fprintf(stderr, "ERROR: Empty deque!\n");
        return NULL_ELEM;
    }
    if (dq_p->tail_iter_p->cur != dq_p->tail_iter_p->first || dq_p->head_iter_p->node == dq_p->tail_iter_p->node)
    { /* tail对应buffer至少有两个元素, 或者 整个deque只有一个buffer时, 也就是deque只有一个元素的情况, pop后deque即将变成空的 */
        T ret = *(dq_p->tail_iter_p->cur);
        _deque_iter_add(dq_p->tail_iter_p, tail, -1);
        return ret;
    }
    else
    { /* tail对应buffer只有一个元素,且和head不在一个buffer */
        T ret = *(dq_p->tail_iter_p->cur);
        free(dq_p->tail_iter_p->node);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
        _deque_iter_set_node(dq_p->tail_iter_p, tail, dq_p->tail_iter_p->node - 1, dq_p->num_elems_each_buffer);
        dq_p->tail_iter_p->cur = dq_p->tail_iter_p->last;
        return ret;
    }
}

static void _deque_push_head_aux(Deque *dq_p, const T *elem_addr)
{
    assert(dq_p != NULL);
    if (dq_p->head_iter_p->node - 1 >= dq_p->map_pointer + 0)
    { /* map还有空间 */

        /*开辟新的buffer*/
        *(dq_p->head_iter_p->node - 1) = _node_alloc(dq_p->num_elems_each_buffer);

        /* 重新设置iter */
        _deque_iter_set_node(dq_p->head_iter_p, head, dq_p->head_iter_p->node - 1, dq_p->num_elems_each_buffer);
        dq_p->head_iter_p->cur = dq_p->head_iter_p->last;

        /* elem添加 */
        memcpy(dq_p->head_iter_p->cur, elem_addr, sizeof(T));
    }
    else
    { /* map空间不足，重新开辟map，并复制原来的 */
    }
}

static void deque_push_head(Deque *dq_p, const T *elem_addr)
{
    assert(dq_p != NULL);

    /* head指向的buffer中有1个或1个以上的剩余空间, STL中是2个 */
    if (dq_p->head_iter_p->cur != dq_p->head_iter_p->first)
    {
        _deque_iter_add(dq_p->head_iter_p, head, -1);
        memcpy(dq_p->head_iter_p->cur, elem_addr, sizeof(T));
    }
    else
    { /*head指向的buffer中0个剩余空间*/
        _deque_push_head_aux(dq_p, elem_addr);
    }
}

static void _deque_push_tail_aux(Deque *dq_p, const T *elem_addr)
{
    assert(dq_p != NULL);
    if (dq_p->tail_iter_p->node + 1 <= dq_p->map_pointer + dq_p->map_size - 1)
    { /* map还有空间 */

        /* 开辟新的buffer*/
        *(dq_p->tail_iter_p->node + 1) = _node_alloc(dq_p->num_elems_each_buffer);

        /* 重新设置iter */
        _deque_iter_set_node(dq_p->tail_iter_p, tail, dq_p->tail_iter_p->node + 1, dq_p->num_elems_each_buffer);
        dq_p->tail_iter_p->cur = dq_p->tail_iter_p->first;

        /* 把elem添加了 */
        memcpy(dq_p->tail_iter_p->cur, elem_addr, sizeof(T));
    }
    else
    { /* 空间不足，重新开辟map，并复制原来的 */
    }
}

static void deque_push_tail(Deque *dq_p, const T *elem_addr)
{
    assert(dq_p != NULL);
    /* tail指向的buffer中有1个或1个以上的剩余空间, STL中是2个 */
    if (dq_p->tail_iter_p->cur != dq_p->tail_iter_p->last)
    {
        _deque_iter_add(dq_p->tail_iter_p, tail, 1);
        memcpy(dq_p->tail_iter_p->cur, elem_addr, sizeof(T));
    }
    else
    { /*tail指向的buffer中0个剩余空间*/
        _deque_push_tail_aux(dq_p, elem_addr);
    }
}

static void deque_insert_i(Deque *dq_p, const deque_size_type index, const T elem)
{
}

static T deque_delete_i(Deque *dq_p, const deque_size_type index)
{
}

static T deque_set_i(Deque *dq_p, const deque_size_type index, const T elem)
{
}

#undef T
#undef NULL_ELEM
/*
#endif
*/