#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct node {
  unsigned long id;
  struct node **edges;
  size_t edges_len;
} node;

typedef struct node_list {
  node **nodes;
  size_t nodes_len;
} node_list;

node *find_or_add_node(parsed_input input, node_list *graph,
                       unsigned long node_id) {
  for (size_t i = 0; i < graph->nodes_len; i++) {
    if (graph->nodes[i]->id == node_id)
      return graph->nodes[i];
  }
  node *new_node = malloc(sizeof(node));
  *new_node = (node){
      .edges = malloc(sizeof(node *) * input.pairs_len),
      .edges_len = 0,
      .id = node_id,
  };
  graph->nodes[graph->nodes_len++] = new_node;
  return graph->nodes[graph->nodes_len - 1];
}

void add_edge(node *before_node, node *after_node) {
  before_node->edges[before_node->edges_len++] = after_node;
}

node_list build_graph(parsed_input input) {
  node **nodes = malloc(sizeof(node *) * input.pairs_len);
  node_list graph = {.nodes = nodes};
  for (size_t i = 0; i < input.pairs_len; i++) {
    ordered_pair pair = input.pairs[i];
    node *before_node = find_or_add_node(input, &graph, pair.before);
    node *after_node = find_or_add_node(input, &graph, pair.after);
    add_edge(before_node, after_node);
  }
  return graph;
}

node *find_node_by_id(unsigned long id, node_list graph) {
  for (size_t i = 0; i < graph.nodes_len; i++) {
    node *n = graph.nodes[i];
    if (n->id == id)
      return n;
  }
  return NULL;
}

node *find_path_to_node_with_id(node *from_node, unsigned long to_node_id) {
  for (size_t i = 0; i < from_node->edges_len; i++) {
    node *neighbour = from_node->edges[i];
    if (neighbour->id == to_node_id)
      return neighbour;
    node *found = find_path_to_node_with_id(neighbour, to_node_id);
    if (found != NULL)
      return found;
  }
  return NULL;
}

bool is_valid_update(update upd, node_list graph) {
  if (upd.len < 2)
    return true;
  node *current_node = find_node_by_id(upd.values[0], graph);
  for (size_t i = 1; i < upd.len; i++) {
    current_node = find_path_to_node_with_id(current_node, upd.values[i]);
    if (current_node == NULL)
      return false;
  }
  return true;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  node_list graph = build_graph(input);

  int sum = 0;

  for (size_t i = 0; i < input.updates_len; i++) {
    update upd = input.updates[i];
    if (is_valid_update(upd, graph)) {
      sum += upd.values[upd.len / 2];
    }
  }

  return sum;
}
