#include "object.h"
#include "my_bool.h"
#ifndef _LINKEDLIST_NOHEAD_H_
#define _LINKEDLIST_NOHEAD_H_

typedef struct Node
{
    /*单链表*/
    Node *next;
    void *object;

    ObjectFuncs *obj_funcs;
} Node;
typedef Node *LinkedList;

LinkedList linked_list_new(size_type size, ObjectFuncs *o);
void **linked_list_calloc_data(size_type size);
LinkedList linked_list_new_from_linked_list(const LinkedList L);
LinkedList linked_list_new_from_args(ObjectFuncs *o, int size, ...);
LinkedList linked_list_new_from_objects(void **p, size_type size, ObjectFuncs *o);
void linked_list_print(const LinkedList L, const char *format);
void linked_list_free(LinkedList L);
void linked_list_free_data(LinkedList L);
size_type linked_list_length(LinkedList L);
void *linked_list_object_at(const LinkedList L, size_type index, my_bool allow_negative_index);
void linked_list_index_assign(const LinkedList L, size_type index, void *object);
size_type linked_list_left_search(const LinkedList L, void *object);
void linked_list_append(LinkedList L, void *object);
void linked_list_join(LinkedList v_left, LinkedList v_right);
void *linked_list_pop(LinkedList L);
void *linked_list_pop_i(LinkedList L, size_type index);
void linked_list_insert(LinkedList L, size_t index, void *object);
void linked_list_deep_copy(const LinkedList v_src, LinkedList v_des);

void linked_list_reverse(LinkedList L, size_type size);
#endif