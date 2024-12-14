#pragma once
#include "./lib.h"
#include "./rational.h"
#include <stdlib.h>

typedef struct vec {
  rational x;
  rational y;
} vec;

typedef struct game {
  vec a;
  vec b;
  vec prize;
} game;

typedef struct parsed_input {
  game *games;
  size_t game_len;
} parsed_input;

parsed_input parse_input(char *input_path);
