#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum node_state {
  UNDISCOVERED = 0,
  DISCOVERED,
  FINISHED,
} node_state;

typedef struct node {
  unsigned long id;
  node_state state;
  struct node **in;
  size_t in_len;
  struct node **out;
  size_t out_len;
} node;

typedef struct node_list {
  node *buffer;
  size_t len;
} node_list;

typedef struct sorted_node_list {
  node **buffer;
  size_t len;
} sorted_node_list;

node *find_start_node(node_list graph) {
  for (size_t i = 0; i < graph.len; i++) {
    node *n = &graph.buffer[i];
    if (n->state == UNDISCOVERED && n->in_len == 0)
      return n;
  }
  return NULL;
}

void dfs(node *start_node, sorted_node_list *result) {
  start_node->state = DISCOVERED;
  for (size_t i = 0; i < start_node->out_len; i++) {
    node *neighbour = start_node->out[i];
    if (neighbour->state == UNDISCOVERED) {
      dfs(start_node->out[i], result);
    }
  }
  start_node->state = FINISHED;
  result->buffer[result->len++] = start_node;
}

sorted_node_list topological_sort(node_list graph) {
  sorted_node_list result = {.buffer = malloc(sizeof(node *) * graph.len)};
  while (true) {
    node *start_node = find_start_node(graph);
    if (start_node == NULL) {
      return result;
    }
    dfs(start_node, &result);
  }
}

node *find_node(unsigned long node_id, node_list nodes) {
  for (size_t i = 0; i < nodes.len; i++) {
    node *n = &nodes.buffer[i];
    if (n->id == node_id)
      return n;
  }
  return NULL;
}

node_list build_graph(parsed_input input, update upd) {
  node_list result = {.buffer = malloc(sizeof(node) * upd.len), .len = 0};
  for (size_t i = 0; i < upd.len; i++) {
    unsigned long node_id = upd.values[i];
    result.buffer[result.len++] = (node){
        .id = node_id,
        .in = malloc(sizeof(node *) * upd.len),
        .out = malloc(sizeof(node *) * upd.len),
    };
  }
  for (size_t i = 0; i < input.pairs_len; i++) {
    ordered_pair pair = input.pairs[i];
    node *before_node = find_node(pair.before, result);
    node *after_node = find_node(pair.after, result);
    if (before_node && after_node) {
      after_node->out[after_node->out_len++] = before_node;
      before_node->in[before_node->in_len++] = after_node;
    }
  }

  return result;
}

bool equals(update upd, sorted_node_list sorted_nodes) {
  for (size_t i = 0; i < upd.len; i++) {
    if (upd.values[i] != sorted_nodes.buffer[i]->id)
      return false;
  }
  return true;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  int sum = 0;

  for (size_t i = 0; i < input.updates_len; i++) {
    update upd = input.updates[i];
    node_list graph = build_graph(input, upd);
    sorted_node_list sorted_nodes = topological_sort(graph);

    if (equals(upd, sorted_nodes)) {
      sum += sorted_nodes.buffer[sorted_nodes.len / 2]->id;
    }
  }

  return sum;
}

int part2(char *input_path) {
  parsed_input input = parse_input(input_path);
  int sum = 0;
  for (size_t i = 0; i < input.updates_len; i++) {
    update upd = input.updates[i];
    node_list graph = build_graph(input, upd);
    sorted_node_list sorted_nodes = topological_sort(graph);
    if (!equals(upd, sorted_nodes)) {
      sum += sorted_nodes.buffer[sorted_nodes.len / 2]->id;
    }
  }

  return sum;
}
