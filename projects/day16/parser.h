#pragma once
#include "./lib.h"

typedef struct parsed_input {
  char **map;
  int map_width;
  int map_height;
} parsed_input;

parsed_input parse_input(char *input_path);
