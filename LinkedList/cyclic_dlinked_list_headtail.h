#include "my_bool.h"
#include "my_debug.h"

#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
#include <stdint.h>
#include <stdio.h>

#ifndef _CYCLIC_DLINKEDLIST_HEADTAIL_H_
#define _CYCLIC_DLINKEDLIST_HEADTAIL_H_

#ifndef LINKEDLIST_INDEX_TYPE
#define LINKEDLIST_INDEX_TYPE
typedef int64_t linked_list_index_type;
#endif

#ifndef LINKEDLIST_SIZE_TYPE
#define LINKEDLIST_SIZE_TYPE
typedef int64_t linked_list_size_type;
#endif

typedef int status; /*状态*/

typedef struct DNode
{
    void *data;
    struct DNode *next;
    struct DNode *prev;
} DNode;

typedef struct DNodeGuard
{
    DNode *head;
    DNode *tail;
    linked_list_size_type size;
} DNodeGuard;

typedef DNodeGuard *DLinkedList;

extern my_bool cyclic_dlinked_list_is_empty(const DLinkedList L);
extern DLinkedList cyclic_dlinked_list_new(linked_list_size_type size);
extern DLinkedList cyclic_dlinked_list_new_from_args(linked_list_size_type size, ...);
extern DLinkedList cyclic_dlinked_list_new_from_elements(void **p, linked_list_size_type size);
extern void cyclic_dlinked_list_print(const DLinkedList L, const char *format, void (*elem_print)(FILE *, const void *));
extern void cyclic_dlinked_list_free(DLinkedList *DLP);
extern linked_list_size_type cyclic_dlinked_list_length(const DLinkedList L);
extern void *cyclic_dlinked_list_right_delete_objects(DLinkedList DL, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *));
extern void *cyclic_dlinked_list_left_delete_objects(DLinkedList DL, const void *elem, linked_list_size_type n, int (*elem_cmp)(const void *, const void *));
extern linked_list_index_type cyclic_dlinked_list_right_search(const DLinkedList L, const void *elem, int (*elem_cmp)(const void *, const void *));
extern linked_list_index_type cyclic_dlinked_list_left_search(const DLinkedList L, const void *elem, int (*elem_cmp)(const void *, const void *));
extern my_bool cyclic_dlinked_list_contains(const DLinkedList L, void *elem, int (*elem_cmp)(const void *, const void *));
extern void cyclic_dlinked_list_append_object(DLinkedList DL, void *elem);
extern void cyclic_dlinked_list_join(DLinkedList DL_left, DLinkedList DL_right);
extern void *cyclic_dlinked_list_pop(DLinkedList DL);
extern void *cyclic_dlinked_list_object_at(const DLinkedList DL, linked_list_index_type index, my_bool allow_negative_index);
extern void cyclic_dlinked_list_insert_object(DLinkedList DL, linked_list_index_type index, void *data);
extern void cyclic_dlinked_list_insert_linked_list(DLinkedList DL_src, linked_list_index_type index, DLinkedList *DL_new);
extern status cyclic_dlinked_list_insert_node(DLinkedList DL, linked_list_index_type index, DNode *p_new_node);
extern void *cyclic_dlinked_list_index_assign(const DLinkedList DL, linked_list_index_type index, my_bool allow_negative_index, void *elem);
extern void *cyclic_dlinked_list_pop_i(DLinkedList DL, linked_list_index_type index, my_bool allow_negative_index);
extern void cyclic_dlinked_list_reverse(DLinkedList DL);
extern DLinkedList cyclic_dlinked_list_slice(DLinkedList L, linked_list_index_type left, linked_list_index_type right, linked_list_index_type step);

#endif