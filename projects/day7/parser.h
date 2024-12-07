#pragma once
#include "./lib.h"
#include <stdlib.h>

typedef struct line {
  unsigned long expected;
  unsigned long *operands;
  size_t operands_len;
} line;

typedef struct parsed_input {
  line *lines;
  size_t len;
} parsed_input;

parsed_input parse_input(char *input_path);
