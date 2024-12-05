#pragma once
#include "./lib.h"
#include <stdlib.h>

typedef struct ordered_pair {
  unsigned long before;
  unsigned long after;
} ordered_pair;

typedef struct update {
  unsigned long *values;
  size_t len;
} update;

typedef struct parsed_input {
  ordered_pair *pairs;
  size_t pairs_len;
  update *updates;
  size_t updates_len;
} parsed_input;

parsed_input parse_input(char *input_path);
