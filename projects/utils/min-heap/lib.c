#include "./lib.h"
#include <stdio.h>

min_heap heap_create(size_t capacity) {
  return (min_heap){
      .buffer = malloc(sizeof(heap_node) * capacity),
      .capacity = capacity,
      .size = 0,
  };
}

void bubble_up(min_heap *heap, size_t i) {
  for (; i > 0; i = (i - 1) / 2) {
    heap_node child = heap->buffer[i];
    heap_node parent = heap->buffer[(i - 1) / 2];
    if (child.key < parent.key) {
      heap->buffer[(i - 1) / 2] = child;
      heap->buffer[i] = parent;
    } else {
      break;
    }
  }
}

void sink_down(min_heap *heap, size_t i) {
  while (2 * i + 1 < heap->size) {
    size_t child_a_idx = 2 * i + 1;
    size_t child_b_idx = 2 * i + 2;
    heap_node parent = heap->buffer[i];
    size_t smallest_child_idx =
        child_b_idx < heap->size &&
                heap->buffer[child_b_idx].key < heap->buffer[child_a_idx].key
            ? child_b_idx
            : child_a_idx;

    heap_node smallest_child = heap->buffer[smallest_child_idx];
    if (parent.key > smallest_child.key) {
      heap->buffer[smallest_child_idx] = parent;
      heap->buffer[i] = smallest_child;
      i = smallest_child_idx;
      continue;
    }
    break;
  }
}

void heap_insert(min_heap *heap, heap_node node) {
  if (heap->size == heap->capacity) {
    fprintf(stderr, "heap is full\n");
    exit(1);
  }
  heap->buffer[heap->size++] = node;
  bubble_up(heap, heap->size - 1);
}

heap_node heap_extract(min_heap *heap) {
  if (heap->size == 0) {
    fprintf(stderr, "can't extract from an empty heap\n");
    exit(1);
  }
  heap_node min = heap->buffer[0];
  heap->buffer[0] = heap->buffer[heap->size - 1];
  heap->size--;
  sink_down(heap, 0);
  return min;
}

void heap_decrease_key(min_heap *heap, size_t i, int val) {
  heap->buffer[i].key = val;
  bubble_up(heap, i);
}
