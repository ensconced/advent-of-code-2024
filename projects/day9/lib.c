#include "./lib.h"
#include <stdio.h>
#include <unistd.h>

void print_list(LinkedList *input) {
  printf("\033[2J\033[1;1H"); // clear terminal
  for (LinkedListNode *curr = input->head; curr != NULL; curr = curr->next) {
    block *blk = curr->data;

    for (size_t i = 0; i < blk->size; i++) {
      if (blk->id == -1) {
        printf(" ");
      } else {
        switch (blk->id % 7) {
        case 6: {
          printf("\033[0;31m");
          break;
        }
        case 5: {
          printf("\033[0;32m");
          break;
        }
        case 4: {
          printf("\033[0;33m");
          break;
        }
        case 3: {
          printf("\033[0;34m");
          break;
        }
        case 2: {
          printf("\033[0;35m");
          break;
        }
        case 1: {
          printf("\033[0;36m");
          break;
        }
        case 0: {
          printf("\033[0;37m");
          break;
        }
        }
        printf("X");
      }
    }
  }
  printf("\n");
  usleep(1000 * 2000);
}

size_t min(size_t a, size_t b) { return a < b ? a : b; }

LinkedListNode *find_block_with_space(LinkedListNode *start_node) {
  for (LinkedListNode *curr = start_node; curr != NULL; curr = curr->next) {
    block *blk = curr->data;
    if (blk->id == -1 && blk->size > 0) {
      return curr;
    }
  }
  return NULL;
}

void move_into_free_space(LinkedListNode *node, LinkedList *list) {
  block *blk = node->data;
  LinkedListNode *first_node_with_space = list->head;

  while (blk->size > 0) {
    print_list(list);
    first_node_with_space = find_block_with_space(first_node_with_space);
    if (first_node_with_space == NULL) {
      printf("found no free node\n");
      return;
    }
    block *blk_with_space = first_node_with_space->data;
    size_t space_to_move = min(blk->size, blk_with_space->size);

    printf("got a block of size %lu ", blk->size);
    printf("and %lu free space. so will move %lu\n", blk_with_space->size,
           space_to_move);

    blk->size -= space_to_move;
    blk_with_space->id = blk->id;

    if (blk_with_space->size > space_to_move) {
      printf("need to make a new free block...");
      block *new_free_block = malloc(sizeof(block));
      *new_free_block =
          (block){.id = -1, .size = blk_with_space->size - space_to_move};
      printf("with size of %lu\n", blk_with_space->size - space_to_move);
      LinkedListNode *new_free_node = malloc(sizeof(LinkedListNode));
      *new_free_node = (LinkedListNode){.data = new_free_block};
      insert_after(first_node_with_space, new_free_node);
    } else {
      printf("not making a new block...\n");
    }
    usleep(1000 * 2000);
    blk_with_space->size = space_to_move;
  }
  remove_node(list, node);
}

unsigned long part1(char *input_path) {
  LinkedList *input = parse_input(input_path);

  for (LinkedListNode *curr = find_tail(input); curr != NULL;
       curr = curr->prev) {
    block *blk = curr->data;
    if (blk->id != -1) {
      move_into_free_space(curr, input);
    }
  }

  unsigned long sum = 0;
  size_t idx = 0;
  for (LinkedListNode *curr = input->head; curr != NULL; curr = curr->next) {
    block *blk = curr->data;
    if (blk->id != -1) {
      for (int i = 0; i < (int)blk->size; i++) {
        sum += (idx++) * (size_t)blk->id;
      }
    }
  }

  return sum;
}
