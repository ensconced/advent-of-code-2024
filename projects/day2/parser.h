#pragma once
#include "./lib.h"
#include <stdlib.h>

typedef struct parsed_line {
  size_t length;
  unsigned long *numbers;
} parsed_line;

typedef struct parsed_input {
  size_t length;
  parsed_line *lines;
} parsed_input;

parsed_input parse_input(char *input_path);
