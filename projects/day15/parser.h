#pragma once
#include "./lib.h"
#include <stdlib.h>

typedef struct parsed_input {
  char *movements;
  size_t movements_len;
  char **map;
  size_t map_height;
  size_t map_width;
} parsed_input;

parsed_input parse_input(char *input_path);
