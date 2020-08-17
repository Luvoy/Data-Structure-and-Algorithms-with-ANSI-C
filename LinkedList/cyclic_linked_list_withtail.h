#include "../my_bool.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#include <stdint.h>
#include <stdio.h>
#ifndef _CYCLIC_LINKEDLIST_WITHTAIL_H_
#define _CYCLIC_LINKEDLIST_WITHTAIL_H_

#ifndef LINKEDLIST_INDEX_TYPE
#define LINKEDLIST_INDEX_TYPE
typedef int64_t linked_list_index_type;
#endif

#ifndef LINKEDLIST_SIZE_TYPE
#define LINKEDLIST_SIZE_TYPE
typedef int64_t linked_list_size_type;
#endif

typedef int status; /*状态*/

typedef struct Node
{
    /*带尾指针的单循环链表*/
    struct Node *next;
    void *data;
} Node;
typedef Node *LinkedList;

extern my_bool linked_list_is_empty(const LinkedList L);
extern LinkedList linked_list_new(linked_list_size_type size);
extern LinkedList linked_list_new_from_args(linked_list_size_type size, ...);
extern LinkedList linked_list_new_from_elements(void **p, linked_list_size_type size);
extern void linked_list_print(const LinkedList L, const char *format, void (*elem_print)(FILE *, const void *));
extern void linked_list_free(LinkedList *Lp);
extern linked_list_size_type linked_list_length(const LinkedList L);
extern void *linked_list_right_delete_objects(LinkedList *Lp, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *));
extern void *linked_list_left_delete_objects(LinkedList *Lp, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *));
extern linked_list_index_type linked_list_right_search(const LinkedList L, const void *elem, int (*elem_cmp)(const void *, const void *));
extern linked_list_index_type linked_list_left_search(const LinkedList L, const void *elem, int (*elem_cmp)(const void *, const void *));
extern my_bool linked_list_contains(const LinkedList L, void *elem, int (*elem_cmp)(const void *, const void *));
extern void linked_list_append_object(LinkedList *Lp, void *elem);
extern void linked_list_join(LinkedList *Lp_left, LinkedList *Lp_right);
extern void *linked_list_pop(LinkedList *Lp);
extern void *linked_list_object_at(const LinkedList L, linked_list_index_type index);
extern void linked_list_insert_object(LinkedList *Lp, linked_list_index_type index, void *data);
extern void linked_list_insert_linked_list(LinkedList *Lp_src, linked_list_index_type index, LinkedList *Lp_new);
extern status linked_list_insert_node(LinkedList *Lp, linked_list_index_type index, Node *p_new_node);
extern void linked_list_index_assign(const LinkedList L, linked_list_index_type index, void *elem);
extern void *linked_list_pop_i(LinkedList *Lp, linked_list_index_type index);
extern void linked_list_reverse(LinkedList *Lp);
extern LinkedList linked_list_slice(LinkedList L, linked_list_index_type left, linked_list_index_type right, linked_list_index_type step);

#endif