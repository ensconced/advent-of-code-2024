#pragma once
#include <stdlib.h>

typedef struct {
  void *data;
  int key;
} heap_node;

typedef struct min_heap {
  heap_node *buffer;
  size_t capacity;
  size_t size;
} min_heap;

min_heap heap_create(size_t capacity);
heap_node heap_extract(min_heap *heap);
void heap_insert(min_heap *heap, heap_node node);
void heap_decrease_key(min_heap *heap, size_t i, int val);
