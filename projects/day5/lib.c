#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct node_list {
  unsigned long *buffer;
  size_t len;
} node_list;

typedef enum node_state {
  UNDISCOVERED = 0,
  DISCOVERED,
  FINISHED,
} node_state;

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

bool *get_adjacencies(parsed_input input, unsigned long max) {
  bool *adjacencies = calloc((max + 1) * (max + 1), sizeof(bool));
  for (size_t i = 0; i < input.pairs_len; i++) {
    ordered_pair pair = input.pairs[i];
    adjacencies[pair.before * max + pair.after] = true;
  }
  return adjacencies;
}

bool is_valid_update(update upd, bool *adjacencies, unsigned long max) {
  for (size_t j = 1; j < upd.len; j++) {
    for (size_t i = 0; i < j; i++) {
      if (adjacencies[upd.values[j] * max + upd.values[i]]) {
        return false;
      }
    }
  }
  return true;
}

bool has_in_edges(bool *adjacencies, unsigned long i, unsigned long max) {
  for (size_t j = 0; j <= max; j++) {
    if (adjacencies[j * max + i])
      return false;
  }
  return true;
}

ssize_t find_start_node(bool *adjacencies, unsigned long max,
                        node_state *node_states) {
  for (unsigned long i = 0; i <= max; i++) {
    if (node_states[i] != FINISHED && !has_in_edges(adjacencies, i, max)) {
      return (ssize_t)i;
    }
  }
  return -1;
}

void dfs(unsigned long node, bool *adjacencies, unsigned long max,
         node_list *sorted, node_state *node_states) {
  node_states[node] = DISCOVERED;
  for (size_t i = 0; i <= max; i++) {
    if (adjacencies[i * max + node] && node_states[i] == UNDISCOVERED) {
      dfs(i, adjacencies, max, sorted, node_states);
    }
  }
  node_states[node] = FINISHED;
  sorted->buffer[sorted->len++] = node;
}

node_list topological_sort(bool *adjacencies, unsigned long max) {
  node_list result = {
      .buffer = malloc(sizeof(unsigned long) * (max + 1)),
      .len = 0,
  };
  node_state *node_states = calloc((max + 1), sizeof(node_state));

  while (true) {
    ssize_t start_node = find_start_node(adjacencies, max, node_states);
    if (start_node == -1)
      return result;
    dfs((unsigned long)start_node, adjacencies, max, &result, node_states);
  }

  return result;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  unsigned long max = max_node_id(input);
  bool *adjacencies = get_adjacencies(input, max);

  int sum = 0;

  for (size_t i = 0; i < input.updates_len; i++) {
    update upd = input.updates[i];
    if (is_valid_update(upd, adjacencies, max)) {
      sum += upd.values[upd.len / 2];
    }
  }

  return sum;
}

bool contains(update upd, unsigned long node) {
  for (size_t i = 0; i < upd.len; i++) {
    if (upd.values[i] == node)
      return true;
  }
  return false;
}

update reorder_update(update upd, node_list sorted_nodes) {
  update result = {
      .len = 0,
      .values = malloc(upd.len * sizeof(unsigned long)),
  };

  for (size_t i = 0; i < sorted_nodes.len; i++) {
    unsigned long node = sorted_nodes.buffer[i];
    if (contains(upd, node)) {
      result.values[result.len++] = node;
    }
  }

  return result;
}

int part2(char *input_path) {
  parsed_input input = parse_input(input_path);
  unsigned long max = max_node_id(input);
  bool *adjacencies = get_adjacencies(input, max);
  node_list sorted = topological_sort(adjacencies, max);

  int sum = 0;

  for (size_t i = 0; i < input.updates_len; i++) {
    update upd = input.updates[i];
    if (!is_valid_update(upd, adjacencies, max)) {
      update reordered = reorder_update(upd, sorted);
      sum += reordered.values[reordered.len / 2];
    }
  }

  return sum;
}
