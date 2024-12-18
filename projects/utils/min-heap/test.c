#include "./lib.h"
#include <assert.h>
#include <stdio.h>

void test_insert_and_extract(void) {
  min_heap heap = heap_create(16);

  heap_insert(&heap, (heap_node){.data = NULL, .key = 4});
  heap_insert(&heap, (heap_node){.data = NULL, .key = 1});
  heap_insert(&heap, (heap_node){.data = NULL, .key = 2});
  heap_insert(&heap, (heap_node){.data = NULL, .key = 3});
  heap_insert(&heap, (heap_node){.data = NULL, .key = 5});
  assert(heap.size == 5);

  assert(heap_extract(&heap).key == 1);
  assert(heap.size == 4);
  assert(heap_extract(&heap).key == 2);
  assert(heap.size == 3);
  assert(heap_extract(&heap).key == 3);
  assert(heap.size == 2);
  assert(heap_extract(&heap).key == 4);
  assert(heap.size == 1);
  assert(heap_extract(&heap).key == 5);
  assert(heap.size == 0);
}

void test_insert_decrease_and_extract(void) {
  min_heap heap = heap_create(16);

  heap_insert(&heap, (heap_node){.data = NULL, .key = 4});
  heap_insert(&heap, (heap_node){.data = NULL, .key = 1});
  heap_insert(&heap, (heap_node){.data = NULL, .key = 2});
  heap_insert(&heap, (heap_node){.data = NULL, .key = 3});
  heap_insert(&heap, (heap_node){.data = NULL, .key = 5});

  size_t idx;
  for (size_t i = 0; i < heap.size; i++) {
    if (heap.buffer[i].key == 5) {
      idx = i;
    }
  }
  heap_decrease_key(&heap, idx, 0);

  assert(heap_extract(&heap).key == 0);
  assert(heap.size == 4);
  assert(heap_extract(&heap).key == 1);
  assert(heap.size == 3);
  assert(heap_extract(&heap).key == 2);
  assert(heap.size == 2);
  assert(heap_extract(&heap).key == 3);
  assert(heap.size == 1);
  assert(heap_extract(&heap).key == 4);
  assert(heap.size == 0);
}

int main(void) {
  printf("testing min heap lib\n");
  test_insert_and_extract();
  test_insert_decrease_and_extract();
}
