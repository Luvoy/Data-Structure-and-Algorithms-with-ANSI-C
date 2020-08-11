#include "../my_bool.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#include <stdint.h>
#include <stdio.h>
#ifndef _LINKEDLIST_WITHHEAD_H_
#define _LINKEDLIST_WITHHEAD_H_

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
    /*有头节点的单链表*/
    struct Node *next;
    void *data;
} Node;
typedef Node *LinkedList;

extern LinkedList linked_list_new(linked_list_size_type size);
extern LinkedList linked_list_new_from_args(linked_list_size_type size, ...);
extern LinkedList linked_list_new_from_elements(void **p, linked_list_size_type size);
extern void linked_list_print(const LinkedList L, const char *format, void (*elem_print)(FILE *, const void *));
extern void linked_list_free(LinkedList *Lp);
extern linked_list_size_type linked_list_length(const LinkedList L);
extern void *linked_list_right_delete_objects(LinkedList L, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *));
extern void *linked_list_left_delete_objects(LinkedList L, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *));
extern linked_list_index_type linked_list_right_search(const LinkedList L, const void *elem, int (*elem_cmp)(const void *, const void *));
extern linked_list_index_type linked_list_left_search(const LinkedList L, const void *elem, int (*elem_cmp)(const void *, const void *));
extern my_bool linked_list_contains(const LinkedList L, void *elem, int (*elem_cmp)(const void *, const void *));
extern void linked_list_append_object(LinkedList L, void *elem);
extern void linked_list_join(LinkedList L_left, LinkedList L_right);
extern void *linked_list_pop(LinkedList L);
extern void *linked_list_object_at(const LinkedList L, linked_list_index_type index);
extern void linked_list_insert_object(LinkedList L, linked_list_index_type index, void *data);
extern void linked_list_insert_linked_list(LinkedList L_src, linked_list_index_type index, LinkedList L_new);
extern status linked_list_insert_node(LinkedList L, linked_list_index_type index, Node *p_new_node);
extern void linked_list_index_assign(const LinkedList L, linked_list_index_type index, void *elem);
extern void *linked_list_pop_i(LinkedList L, linked_list_index_type index);
extern void linked_list_reverse(LinkedList L);
extern LinkedList linked_list_slice(LinkedList L, linked_list_index_type left, linked_list_index_type right, linked_list_index_type step);
#endif