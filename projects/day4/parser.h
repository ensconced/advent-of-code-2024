#pragma once
#include "./lib.h"
#include <stdlib.h>

typedef struct parsed_input {
  char **lines;
  size_t height;
  size_t width;
} parsed_input;

parsed_input parse_input(char *input_path);
