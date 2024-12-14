#pragma once
#include "./lib.h"
#include <stdlib.h>

typedef struct vec {
  int x;
  int y;
} vec;

typedef struct robot {
  vec position;
  vec velocity;
} robot;

typedef struct parsed_input {
  robot *robots;
  size_t len;
} parsed_input;

parsed_input parse_input(char *input_path);
