#pragma once
#include "./lib.h"
#include <stdlib.h>

typedef struct parsed_input {
  long a;
  long b;
  long c;
  int *program;
  size_t program_len;
} parsed_input;

parsed_input parse_input(char *input_path);
