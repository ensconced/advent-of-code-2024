#include "./lib.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void print_list(LinkedList *block_list) {
  printf("\033[2J\033[1;1H"); // clear terminal
  for (LinkedListNode *curr = block_list->head; curr != NULL;
       curr = curr->next) {
    block *blk = curr->data;

    for (size_t i = 0; i < blk->size; i++) {
      if (blk->id == -1) {
        printf("\033[0;37m");
        printf(".");

      } else {
        switch (blk->id % 6) {
        case 5: {
          printf("\033[0;31m");
          break;
        }
        case 4: {
          printf("\033[0;32m");
          break;
        }
        case 3: {
          printf("\033[0;33m");
          break;
        }
        case 2: {
          printf("\033[0;34m");
          break;
        }
        case 1: {
          printf("\033[0;35m");
          break;
        }
        case 0: {
          printf("\033[0;36m");
          break;
        }
        }
        printf("X");
      }
    }
  }
  printf("\n");
  usleep(1000 * 5000);
}

size_t min(size_t a, size_t b) { return a < b ? a : b; }

LinkedListNode *find_free_block_within_range(LinkedListNode *start_node,
                                             LinkedListNode *end_node) {
  for (LinkedListNode *curr = start_node; curr != NULL && curr != end_node;
       curr = curr->next) {
    block *blk = curr->data;
    if (blk->id == -1 && blk->size > 0)
      return curr;
  }
  return NULL;
}

LinkedListNode *find_free_block_by_min_size(LinkedListNode *start_node,
                                            size_t min_size) {
  for (LinkedListNode *curr = start_node; curr != NULL; curr = curr->next) {
    block *blk = curr->data;
    if (blk->id == -1 && blk->size >= min_size)
      return curr;
  }
  return NULL;
}

void distribute_file_into_free_block(LinkedList *list,
                                     LinkedListNode *free_node,
                                     block *file_block) {
  // print_list(list);
  block *free_block = free_node->data;
  size_t amount_to_move = min(file_block->size, free_block->size);

  file_block->size -= amount_to_move;
  free_block->id = file_block->id;

  if (free_block->size > amount_to_move) {
    block *new_free_block = malloc(sizeof(block));
    *new_free_block =
        (block){.id = -1, .size = free_block->size - amount_to_move};
    LinkedListNode *new_free_node = malloc(sizeof(LinkedListNode));
    *new_free_node = (LinkedListNode){.data = new_free_block};
    insert_after(free_node, new_free_node);
  }
  free_block->size = amount_to_move;
  file_block->moved = true;
}

void compact_file_allowing_fragmentation(LinkedListNode *file_node,
                                         LinkedListNode **free_node_search_head,
                                         LinkedList *list) {
  block *file_block = file_node->data;

  while (file_block->size > 0) {
    *free_node_search_head =
        find_free_block_within_range(*free_node_search_head, file_node);
    if (*free_node_search_head == NULL)
      return;
    distribute_file_into_free_block(list, *free_node_search_head, file_block);
  }
  remove_node(list, file_node);
}

void move_entire_file_into_free_block(LinkedListNode *file_node,
                                      LinkedList *list) {
  block *file_block = file_node->data;
  LinkedListNode *free_node =
      find_free_block_by_min_size(list->head, file_block->size);
  if (free_node) {
    distribute_file_into_free_block(list, free_node, file_block);
  }
}

unsigned long checksum(LinkedList *block_list) {
  unsigned long sum = 0;
  size_t idx = 0;
  for (LinkedListNode *curr = block_list->head; curr != NULL;
       curr = curr->next) {
    block *blk = curr->data;
    if (blk->id != -1) {
      for (int i = 0; i < (int)blk->size; i++) {
        sum += (idx++) * (size_t)blk->id;
      }
    }
  }
  return sum;
}

void compact_files(LinkedList *block_list, bool allow_fragmentation) {
  LinkedListNode *free_node_search_head = block_list->head;

  for (LinkedListNode *curr = find_tail(block_list);
       curr != NULL && free_node_search_head != NULL; curr = curr->prev) {
    block *blk = curr->data;
    if (blk->id != -1 && !blk->moved) {
      if (allow_fragmentation) {
        compact_file_allowing_fragmentation(curr, &free_node_search_head,
                                            block_list);
      } else {
        move_entire_file_into_free_block(curr, block_list);
        blk->moved = true;
      }
    }
  }
}

unsigned long part1(char *input_path) {
  LinkedList *block_list = parse_input(input_path);
  compact_files(block_list, true);
  return checksum(block_list);
}

// TODO - when moving a file block into free blocks, it should leave free space
// that can then be re-used...
unsigned long part2(char *input_path) {
  LinkedList *block_list = parse_input(input_path);
  compact_files(block_list, false);
  return checksum(block_list);
}
