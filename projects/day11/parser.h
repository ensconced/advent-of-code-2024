#pragma once
#include "./lib.h"
#include <stdlib.h>

typedef struct parsed_input {
  unsigned long long *numbers;
  size_t len;
} parsed_input;

parsed_input parse_input(char *input_path);
