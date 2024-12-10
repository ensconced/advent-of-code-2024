#pragma once
#include "../utils/linked_list/lib.h"
#include "./lib.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct block {
  int id; // -1 means it's a free block
  size_t size;
  bool moved;
} block;

LinkedList *parse_input(char *input_path);
