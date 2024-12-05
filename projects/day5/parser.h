#pragma once
#include "./lib.h"
#include <stdlib.h>

typedef struct edge {
  unsigned long before;
  unsigned long after;
} edge;

typedef struct update {
  unsigned long *values;
  size_t len;
} update;

typedef struct parsed_input {
  edge *edges;
  size_t edges_len;
  update *updates;
  size_t updates_len;
} parsed_input;

parsed_input parse_input(char *input_path);
