#pragma once
#include <stddef.h>

typedef struct LinkedListNode LinkedListNode;
struct LinkedListNode {
  void *data;
  LinkedListNode *prev;
  LinkedListNode *next;
};

typedef struct {
  LinkedListNode *head;
} LinkedList;

LinkedListNode *find_tail(LinkedList *list);
void append(LinkedList *list, LinkedListNode *new_tail);
void remove_node(LinkedList *list, LinkedListNode *node);
void insert_after(LinkedListNode *ref_node, LinkedListNode *new_node);
void insert_before(LinkedList *list, LinkedListNode *ref_node,
                   LinkedListNode *new_node);
void add_at_head(LinkedList *list, LinkedListNode *new_node);
size_t length(LinkedList *list);
