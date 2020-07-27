#include "../object.h"
#include "../my_bool.h"
#ifndef _LINKEDLIST_WITHHEAD_H_
#define _LINKEDLIST_WITHHEAD_H_

typedef struct Node
{
    /*有头节点的单链表*/
    struct Node *next;
    void *data;
} Node;
typedef Node *LinkedList;

LinkedList linked_list_new(size_type size, ObjectFuncs *o);
LinkedList linked_list_new_from_args(ObjectFuncs *o, int size, ...);
LinkedList linked_list_new_from_objects(void **p, size_type size, ObjectFuncs *o);
void linked_list_print(const LinkedList L, const char *format);
void linked_list_free(LinkedList L);

size_type linked_list_length(const LinkedList L);

my_bool linked_list_contains(const LinkedList L, void *obj);

void linked_list_append_object(LinkedList L, void *obj);
void linked_list_join(LinkedList L_left, LinkedList L_right);
void *linked_list_pop(LinkedList L);
void *linked_list_object_at(const LinkedList L, size_type index);
Node *node_new(void *object);
void linked_list_insert_object(LinkedList L, size_type index, void *data);
void linked_list_insert_node(LinkedList L, size_type index, Node *p_new_node);
void node_insert(Node *p_node_left, Node *p_node_new, Node *p_node_right);
void linked_list_index_assign(const LinkedList L, size_type index, void *object);
void *linked_list_pop_i(LinkedList L, size_type index);
void linked_list_reverse(LinkedList L);
void linked_list_insert_linked_list(LinkedList L_src, size_type index, LinkedList L_new);
size_type linked_list_left_search(const LinkedList L, const void *obj);
size_type linked_list_right_search(const LinkedList L, const void *obj);
void *linked_list_left_delete_objects(LinkedList L, const void *obj, size_type n);
void *linked_list_right_delete_objects(LinkedList L, const void *obj, size_type n);
LinkedList linked_list_slice(LinkedList L, size_type left, size_type right, size_type step);
#endif