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
        if (i == 0) {
          printf("[");
        }
        printf(" ");
        if (i == blk->size - 1) {
          printf("]");
        }

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
  usleep(1000 * 1000);
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

void distribute_file_into_free_block(LinkedListNode *free_node,
                                     block *file_block) {
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
}

void distribute_file_into_free_blocks(LinkedListNode *file_node,
                                      LinkedListNode **free_node_search_head,
                                      LinkedList *list) {
  block *file_block = file_node->data;

  while (file_block->size > 0) {
    *free_node_search_head =
        find_free_block_within_range(*free_node_search_head, file_node);
    if (*free_node_search_head == NULL)
      return;
    distribute_file_into_free_block(*free_node_search_head, file_block);
  }
  remove_node(list, file_node);
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

unsigned long part1(char *input_path) {
  LinkedList *block_list = parse_input(input_path);
  LinkedListNode *free_node_search_head = block_list->head;

  for (LinkedListNode *curr = find_tail(block_list);
       curr != NULL && free_node_search_head != NULL; curr = curr->prev) {
    block *blk = curr->data;
    if (blk->id != -1) {
      distribute_file_into_free_blocks(curr, &free_node_search_head,
                                       block_list);
    }
  }

  return checksum(block_list);
}
