#include "./lib.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

size_t min(size_t a, size_t b) { return a < b ? a : b; }

LinkedListNode *find_free_block(LinkedList *list, LinkedListNode *end_node,
                                size_t min_size) {
  for (LinkedListNode *curr = list->head; curr != NULL && curr != end_node;
       curr = curr->next) {
    block *blk = curr->data;
    if (blk->id == -1 && blk->size >= min_size)
      return curr;
  }
  return NULL;
}

void move_portion_into_first_free_block(LinkedListNode *file_node,
                                        LinkedList *list, size_t min_size) {
  LinkedListNode *free_node = find_free_block(list, file_node, min_size);
  if (free_node == NULL)
    return;

  block *file_block = file_node->data;
  block *free_block = free_node->data;

  size_t amount_to_move = min(file_block->size, free_block->size);

  size_t free_block_remainder = free_block->size - amount_to_move;
  if (free_block_remainder > 0) {
    // we need a new block to contain the remaining free space
    block *remainder_free_block = malloc(sizeof(block));
    *remainder_free_block = (block){.size = free_block_remainder, .id = -1};
    LinkedListNode *remainder_free_node = malloc(sizeof(LinkedListNode));
    *remainder_free_node = (LinkedListNode){.data = remainder_free_block};
    insert_after(free_node, remainder_free_node);
  }

  size_t file_block_remainder = file_block->size - amount_to_move;
  if (file_block_remainder > 0) {
    // we need a new block to contain what we couldn't move from
    // the file node
    block *remainder_file_block = malloc(sizeof(block));
    *remainder_file_block =
        (block){.size = file_block_remainder, .id = file_block->id};
    LinkedListNode *remainder_file_node = malloc(sizeof(LinkedListNode));
    *remainder_file_node = (LinkedListNode){.data = remainder_file_block};
    insert_before(list, file_node, remainder_file_node);
  }

  // repurpose the free block as it now contains the file data
  free_block->id = file_block->id;
  free_block->size = amount_to_move;
  free_block->moved = true;

  // repurpose the file block as the new free block
  file_block->id = -1;
  file_block->size = amount_to_move;
}

unsigned long checksum(LinkedList *block_list) {
  unsigned long sum = 0;
  size_t idx = 0;
  for (LinkedListNode *curr = block_list->head; curr != NULL;
       curr = curr->next) {
    block *blk = curr->data;
    for (int i = 0; i < (int)blk->size; i++) {
      if (blk->id != -1) {
        sum += idx * (size_t)blk->id;
      }
      idx++;
    }
  }
  return sum;
}

void compact_files(LinkedList *block_list, bool part2) {
  for (LinkedListNode *curr = find_tail(block_list); curr != NULL;
       curr = curr->prev) {
    block *blk = curr->data;
    if (blk->id != -1 && !(part2 && blk->moved)) {
      move_portion_into_first_free_block(curr, block_list,
                                         part2 ? blk->size : 0);
    }
  }
}

unsigned long part1(char *input_path) {
  LinkedList *block_list = parse_input(input_path);
  compact_files(block_list, false);
  return checksum(block_list);
}

unsigned long part2(char *input_path) {
  LinkedList *block_list = parse_input(input_path);
  compact_files(block_list, true);
  return checksum(block_list);
}
