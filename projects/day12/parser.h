#pragma once
#include "./lib.h"
#include <stdlib.h>

typedef struct parsed_input {
  int height;
  int width;
  char **rows;
} parsed_input;

parsed_input parse_input(char *input_path);
