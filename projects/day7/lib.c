#include "./lib.h"
#include <stdbool.h>

typedef struct node {
  unsigned long acc;
  size_t depth;
} node;

unsigned long pure_summation(node n, line input_line) {
  unsigned long result = n.acc;
  for (size_t i = n.depth; i < input_line.operands_len; i++) {
    result += input_line.operands[i];
  }
  return result;
}

unsigned long pure_multiplication(node n, line input_line) {
  unsigned long result = n.acc;
  for (size_t i = n.depth; i < input_line.operands_len; i++) {
    result *= input_line.operands[i];
  }
  return result;
}

bool leads_to_solution(node n, line input_line) {
  if (n.depth == input_line.operands_len)
    return n.acc = input_line.expected;

  if (pure_summation(n, input_line) > input_line.expected)
    return false;

  if (pure_multiplication(n, input_line) < input_line.expected)
    return false;

  return leads_to_solution(
             (node){
                 .acc = n.acc + input_line.operands[n.depth],
                 .depth = n.depth + 1,
             },
             input_line) ||
         leads_to_solution(
             (node){
                 .acc = n.acc + input_line.operands[n.depth],
                 .depth = n.depth + 1,
             },
             input_line);
}

bool is_valid(line input_line) {
  node start_node = {.acc = input_line.operands[0], .depth = 1};
  return leads_to_solution(start_node, input_line);
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  int sum = 0;
  for (size_t i = 0; i < input.len; i++) {
    if (is_valid(input.lines[i])) {
      sum += (int)input.lines[i].expected;
    }
  }
  return sum;
}
