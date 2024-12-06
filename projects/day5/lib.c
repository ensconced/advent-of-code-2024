#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>

unsigned long max_node_id(parsed_input input) {
  unsigned long max = 0;
  for (size_t i = 0; i < input.pairs_len; i++) {
    ordered_pair pair = input.pairs[i];
    if (pair.before > max)
      max = pair.before;
    if (pair.after > max)
      max = pair.after;
  }
  return max;
}

bool *build_adjacency_matrix(parsed_input input, unsigned long max) {
  bool *adjacency_matrix = calloc((max + 1) * (max + 1), sizeof(bool));
  for (size_t i = 0; i < input.pairs_len; i++) {
    ordered_pair pair = input.pairs[i];
    adjacency_matrix[pair.before * max + pair.after] = true;
  }
  return adjacency_matrix;
}

bool is_valid_update(update upd, bool *adjacency_matrix, unsigned long max) {
  for (size_t j = 1; j < upd.len; j++) {
    for (size_t i = 0; i < j; i++) {
      if (adjacency_matrix[upd.values[j] * max + upd.values[i]]) {
        return false;
      }
    }
  }
  return true;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  unsigned long max = max_node_id(input);
  bool *adjacency_matrix = build_adjacency_matrix(input, max);

  int sum = 0;

  for (size_t i = 0; i < input.updates_len; i++) {
    update upd = input.updates[i];
    if (is_valid_update(upd, adjacency_matrix, max)) {
      sum += upd.values[upd.len / 2];
    }
  }

  return sum;
}
