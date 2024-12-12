#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef enum node_state {
  UNDISCOVERED,
  DISCOVERED,
  FINISHED,
} node_state;

int nines_found(int x, int y, parsed_input input, node_state *node_states) {
  int result = 0;
  node_states[y * (int)input.width + x] = DISCOVERED;
  char a = input.rows[y][x];

  if (a == '9')
    return 1;

  int offsets[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

  for (size_t i = 0; i < 4; i++) {
    int bx = x + offsets[i][0];
    int by = y + offsets[i][1];

    if (bx >= 0 && bx < (int)input.width && by >= 0 && by < (int)input.height) {
      char b = input.rows[by][bx];
      if (node_states[by * (int)input.width + bx] == UNDISCOVERED) {
        if (b - a == 1) {
          result += nines_found(bx, by, input, node_states);
        }
      }
    }
  }

  node_states[y * (int)input.width + x] = FINISHED;
  return result;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  node_state *node_states =
      calloc(input.height * input.width, sizeof(node_state));

  int sum = 0;
  for (int y = 0; y < (int)input.height; y++) {
    for (int x = 0; x < (int)input.width; x++) {
      if (input.rows[y][x] == '0') {
        memset(node_states, 0, input.height * input.width * sizeof(node_state));
        sum += nines_found(x, y, input, node_states);
      }
    }
  }

  return sum;
}
