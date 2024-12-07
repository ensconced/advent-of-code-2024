#include "./lib.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct node {
  unsigned long acc;
  size_t depth;
} node;

unsigned long min(node n, line input_line) {
  unsigned long result = n.acc;
  for (size_t i = n.depth; i < input_line.operands_len; i++) {
    unsigned long mult_result = result * input_line.operands[i];
    unsigned long sum_result = result + input_line.operands[i];
    if (mult_result > sum_result) {
      result = sum_result;
    } else {
      result = mult_result;
    }
  }
  return result;
}

unsigned long max(node n, line input_line) {
  unsigned long result = n.acc;
  for (size_t i = n.depth; i < input_line.operands_len; i++) {
    unsigned long mult_result = result * input_line.operands[i];
    unsigned long sum_result = result + input_line.operands[i];
    if (mult_result < sum_result) {
      result = sum_result;
    } else {
      result = mult_result;
    }
  }
  return result;
}

bool leads_to_solution(node n, line input_line) {
  if (n.depth == input_line.operands_len)
    return n.acc == input_line.expected;

  if (min(n, input_line) > input_line.expected)
    return false;

  if (max(n, input_line) < input_line.expected)
    return false;

  return leads_to_solution(
             (node){
                 .acc = n.acc + input_line.operands[n.depth],
                 .depth = n.depth + 1,
             },
             input_line) ||
         leads_to_solution(
             (node){
                 .acc = n.acc * input_line.operands[n.depth],
                 .depth = n.depth + 1,
             },
             input_line);
}

bool is_valid(line input_line) {
  node start_node = {.acc = input_line.operands[0], .depth = 1};
  return leads_to_solution(start_node, input_line);
}

unsigned long part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  unsigned long sum = 0;
  for (size_t i = 0; i < input.len; i++) {
    if (is_valid(input.lines[i])) {
      sum += input.lines[i].expected;
    }
  }
  return sum;
}
