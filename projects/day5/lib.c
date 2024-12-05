#include "./lib.h"
#include "./parser.h"
#include <stdio.h>

void part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  for (size_t i = 0; i < input.edges_len; i++) {
    printf("%lu|%lu\n", input.edges[i].before, input.edges[i].after);
  }

  for (size_t i = 0; i < input.updates_len; i++) {
    update upd = input.updates[i];
    for (size_t j = 0; j < upd.len; j++) {
      printf("%lu,", upd.values[j]);
    }
    printf("\n");
  }
}
