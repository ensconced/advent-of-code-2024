#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
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
  int shortest_path_weight;
  struct state_node *predecessor;
} state_node;

typedef struct state_graph {
  state_node *nodes;
  size_t nodes_len;
  state_node *start_node;
  state_node **end_nodes;
  size_t end_nodes_len;
} state_graph;

// TODO - could be just a pointer?
typedef struct {
  state_node *graph_node;
} heap_node;

typedef struct min_heap {
  heap_node *buffer;
  size_t capacity;
  size_t size;
} min_heap;

min_heap heap_create(size_t capacity) {
  return (min_heap){
      .buffer = malloc(sizeof(heap_node) * capacity),
      .capacity = capacity,
      .size = 0,
  };
}

void swap_heap_nodes(min_heap *heap, size_t i, size_t j) {
  heap_node node_i = heap->buffer[i];
  heap_node node_j = heap->buffer[j];
  node_i.graph_node->heap_idx = j;
  node_j.graph_node->heap_idx = i;
  heap->buffer[j] = node_i;
  heap->buffer[i] = node_j;
}

void bubble_up(min_heap *heap, size_t i) {
  for (; i > 0; i = (i - 1) / 2) {
    heap_node child = heap->buffer[i];
    heap_node parent = heap->buffer[(i - 1) / 2];
    if (child.graph_node->shortest_path_weight <
        parent.graph_node->shortest_path_weight) {
      swap_heap_nodes(heap, (i - 1) / 2, i);
    } else {
      break;
    }
  }
}

void sink_down(min_heap *heap, size_t i) {
  while (2 * i + 1 < heap->size) {
    size_t child_a_idx = 2 * i + 1;
    size_t child_b_idx = 2 * i + 2;
    heap_node parent = heap->buffer[i];
    size_t smallest_child_idx =
        child_b_idx < heap->size &&
                heap->buffer[child_b_idx].graph_node->shortest_path_weight <
                    heap->buffer[child_a_idx].graph_node->shortest_path_weight
            ? child_b_idx
            : child_a_idx;

    heap_node smallest_child = heap->buffer[smallest_child_idx];
    if (parent.graph_node->shortest_path_weight >
        smallest_child.graph_node->shortest_path_weight) {
      swap_heap_nodes(heap, smallest_child_idx, i);
      i = smallest_child_idx;
      continue;
    }
    break;
  }
}

void heap_insert(min_heap *heap, heap_node node) {
  if (heap->size == heap->capacity) {
    fprintf(stderr, "heap is full\n");
    exit(1);
  }
  heap->buffer[heap->size++] = node;
  node.graph_node->heap_idx = heap->size - 1;
  bubble_up(heap, heap->size - 1);
}

heap_node heap_extract(min_heap *heap) {
  if (heap->size == 0) {
    fprintf(stderr, "can't extract from an empty heap\n");
    exit(1);
  }
  heap_node min = heap->buffer[0];
  heap->buffer[0] = heap->buffer[heap->size - 1];
  heap->size--;
  sink_down(heap, 0);
  return min;
}

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
      .shortest_path_weight = INT32_MAX,
      .predecessor = NULL,
  };
}

void create_edge(state_node *from, state_node *to, int weight) {
  from->out_edges[from->out_edges_len++] =
      (edge){.target = to, .weight = weight};
}

// order matters here for the bsearch comparison function - these are in order
// of increasing ascii codepoint
const char directions[4] = {'<', '>', '^', 'v'};

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

int compare_nodes(const void *key, const void *candidate) {
  state_node *key_node = (state_node *)key;
  state_node *candidate_node = (state_node *)candidate;
  if (key_node->position.y != candidate_node->position.y)
    return key_node->position.y - candidate_node->position.y;
  if (key_node->position.x != candidate_node->position.x)
    return key_node->position.x - candidate_node->position.x;
  if (key_node->direction != candidate_node->direction)
    return key_node->direction - candidate_node->direction;
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
            start_node = &nodes[nodes_len - 1];
            start_node->shortest_path_weight = 0;
          } else if (ch == 'E') {
            end_nodes[end_nodes_len++] = &nodes[nodes_len - 1];
          }
        }

        create_edge(&nodes[nodes_len - 4], &nodes[nodes_len - 2], 1000);
        create_edge(&nodes[nodes_len - 4], &nodes[nodes_len - 1], 1000);
        create_edge(&nodes[nodes_len - 3], &nodes[nodes_len - 2], 1000);
        create_edge(&nodes[nodes_len - 3], &nodes[nodes_len - 1], 1000);
        create_edge(&nodes[nodes_len - 2], &nodes[nodes_len - 4], 1000);
        create_edge(&nodes[nodes_len - 2], &nodes[nodes_len - 3], 1000);
        create_edge(&nodes[nodes_len - 1], &nodes[nodes_len - 4], 1000);
        create_edge(&nodes[nodes_len - 1], &nodes[nodes_len - 3], 1000);
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

void relax_neighbours(state_node *node, min_heap *heap) {
  for (size_t i = 0; i < node->out_edges_len; i++) {
    edge e = node->out_edges[i];
    if ((long)node->shortest_path_weight + (long)e.weight <
        (long)e.target->shortest_path_weight) {
      e.target->shortest_path_weight = node->shortest_path_weight + e.weight;
      e.target->predecessor = node;
      bubble_up(heap, e.target->heap_idx);
    }
  }
}

void dijkstra(min_heap *heap) {
  while (heap->size > 0) {
    heap_node next = heap_extract(heap);
    relax_neighbours(next.graph_node, heap);
  }
}

min_heap build_heap(state_graph graph) {
  min_heap heap = heap_create(graph.nodes_len);
  for (size_t i = 0; i < graph.nodes_len; i++) {
    heap_node node = (heap_node){.graph_node = &graph.nodes[i]};
    heap_insert(&heap, node);
  }
  return heap;
}

int shortest_path_length(state_graph graph) {
  int min_shortest_path = INT32_MAX;
  for (size_t i = 0; i < graph.end_nodes_len; i++) {
    if (graph.end_nodes[i]->shortest_path_weight < min_shortest_path) {
      min_shortest_path = graph.end_nodes[i]->shortest_path_weight;
    }
  }

  return min_shortest_path;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  state_graph graph = build_state_graph(input);
  min_heap heap = build_heap(graph);
  dijkstra(&heap);
  return shortest_path_length(graph);
}

bool search_for_shortest_path(state_node *node, state_graph graph,
                              parsed_input input, bool *on_shortest_path,
                              int path_len) {
  if (path_len > node->shortest_path_weight)
    return false;

  if (input.map[node->position.y][node->position.x] == 'E') {
    on_shortest_path[node->position.y * input.map_width + node->position.x] =
        true;
    return true;
  }

  bool any_neighbour_leads_to_shortest_path = false;
  for (size_t i = 0; i < node->out_edges_len; i++) {
    edge e = node->out_edges[i];
    if (search_for_shortest_path(e.target, graph, input, on_shortest_path,
                                 path_len + e.weight)) {
      any_neighbour_leads_to_shortest_path = true;
      on_shortest_path[node->position.y * input.map_width + node->position.x] =
          true;
    }
  }
  return any_neighbour_leads_to_shortest_path;
}

int part2(char *input_path) {
  // TODO - build matrix to track path counts for each position...
  // ...then do full search, pruning wherever path length at given node exceeds
  // shortest path length to that node
  parsed_input input = parse_input(input_path);
  state_graph graph = build_state_graph(input);
  min_heap heap = build_heap(graph);
  dijkstra(&heap);

  bool *on_shortest_path =
      calloc((size_t)input.map_height * (size_t)input.map_width, sizeof(bool));

  search_for_shortest_path(graph.start_node, graph, input, on_shortest_path, 0);

  int count = 0;
  for (int y = 0; y < input.map_height; y++) {
    for (int x = 0; x < input.map_width; x++) {
      if (on_shortest_path[y * input.map_width + x]) {
        count++;
      }
    }
  }

  return count;
}
