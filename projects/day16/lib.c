#include "./lib.h"
#include "./parser.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct vec {
  int x;
  int y;
} vec;

typedef struct edge {
  int weight;
  struct state_node *target;
} edge;

typedef struct state_node {
  char direction;
  vec position;
  edge *out_edges;
  size_t out_edges_len;
  size_t heap_idx;
} state_node;

typedef struct state_graph {
  state_node *nodes;
  size_t nodes_len;
  state_node *start_node;
  state_node **end_nodes;
  size_t end_nodes_len;
} state_graph;

state_node create_node(char direction, int x, int y) {
  // there are a max of three edges: move forwards, rotate clockwise, rotate
  // anticlockwise
  edge *out_edges = malloc(sizeof(edge) * 3);
  return (state_node){
      .direction = direction,
      .out_edges = out_edges,
      .out_edges_len = 0,
      .heap_idx = 0,
      .position = (vec){.x = x, .y = y},
  };
}

void create_edge(state_node *from, state_node *to, int weight) {
  from->out_edges[from->out_edges_len++] =
      (edge){.target = to, .weight = weight};
}

const char directions[4] = {'>', 'v', '<', '^'};

vec direction_vec(char dir) {
  switch (dir) {
  case '>':
    return (vec){.x = 1, .y = 0};
  case 'v':
    return (vec){.x = 0, .y = 1};
  case '<':
    return (vec){.x = -1, .y = 0};
  case '^':
    return (vec){.x = 0, .y = -1};
  }
  fprintf(stderr, "invalid direction: %c\n", dir);
  exit(1);
}

int compare_nodes(const void *a, const void *b) {
  state_node *node_a = (state_node *)a;
  state_node *node_b = (state_node *)b;
  if (node_a->position.y != node_b->position.y)
    return node_a->position.y - node_b->position.y;
  if (node_a->position.x != node_b->position.x)
    return node_a->position.x - node_b->position.x;
  if (node_a->direction != node_b->direction)
    return node_a->direction - node_b->direction;
  return 0;
}

state_node *find_translation_neighbour(state_node *from, state_node *nodes,
                                       size_t nodes_len) {
  vec step = direction_vec(from->direction);
  state_node match = {
      .direction = from->direction,
      .position =
          (vec){
              .x = from->position.x + step.x,
              .y = from->position.y + step.y,
          },
  };
  return bsearch(&match, nodes, nodes_len, sizeof(state_node), compare_nodes);
}

state_graph build_state_graph(parsed_input input) {
  state_node *start_node;
  // there are four possible end states because we can end in any of the four
  // directions
  state_node **end_nodes = malloc(sizeof(state_node *) * 4);
  size_t end_nodes_len = 0;
  state_node *nodes = malloc(sizeof(state_node) * (size_t)input.map_height *
                             (size_t)input.map_width * 4);
  size_t nodes_len = 0;

  for (int y = 0; y < input.map_height; y++) {
    for (int x = 0; x < input.map_width; x++) {
      char ch = input.map[y][x];
      if (ch == 'S' || ch == '.' || ch == 'E') {
        for (int i = 0; i < 4; i++) {
          nodes[nodes_len++] = create_node(directions[i], x, y);
          if (ch == 'S' && directions[i] == '>') {
            start_node = &nodes[nodes_len];
          } else if (ch == 'E') {
            end_nodes[end_nodes_len++] = &nodes[nodes_len];
          }
        }

        // create rotation edges
        create_edge(&nodes[nodes_len - 1], &nodes[nodes_len - 2], 1000);
        create_edge(&nodes[nodes_len - 2], &nodes[nodes_len - 1], 1000);
        create_edge(&nodes[nodes_len - 2], &nodes[nodes_len - 3], 1000);
        create_edge(&nodes[nodes_len - 3], &nodes[nodes_len - 2], 1000);
        create_edge(&nodes[nodes_len - 3], &nodes[nodes_len - 4], 1000);
        create_edge(&nodes[nodes_len - 4], &nodes[nodes_len - 3], 1000);
        create_edge(&nodes[nodes_len - 4], &nodes[nodes_len - 1], 1000);
        create_edge(&nodes[nodes_len - 1], &nodes[nodes_len - 4], 1000);
      }
    }
  }

  for (size_t i = 0; i < nodes_len; i++) {
    // create translation edges
    state_node *from = &nodes[i];
    state_node *to = find_translation_neighbour(from, nodes, nodes_len);
    if (to != NULL) {
      create_edge(from, to, 1);
    }
  }

  return (state_graph){
      .nodes = nodes,
      .nodes_len = nodes_len,
      .end_nodes = end_nodes,
      .end_nodes_len = end_nodes_len,
      .start_node = start_node,
  };
}

void print_graph(state_graph graph) {
  for (size_t i = 0; i < graph.nodes_len; i++) {
    state_node node = graph.nodes[i];
    printf("(%d, %d) %c\n", node.position.x, node.position.y, node.direction);
    for (size_t j = 0; j < node.out_edges_len; j++) {
      edge e = node.out_edges[j];
      printf(" -> (%d, %d) %c\n", e.target->position.x, e.target->position.y,
             e.target->direction);
    }
  }
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);

  state_graph graph = build_state_graph(input);
  print_graph(graph);
  return 0;
}

int part2(char *input_path) { return 0; }
