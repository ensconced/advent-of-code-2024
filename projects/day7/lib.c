#include "./lib.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct node {
  unsigned long acc;
  size_t depth;
} node;

unsigned long max(unsigned long a, unsigned long b, unsigned long c) {
  if (a >= b && a >= c)
    return a;
  if (b >= a && b >= c)
    return b;
  return c;
}

unsigned long min(unsigned long a, unsigned long b, unsigned long c) {
  if (a <= b && a <= c)
    return a;
  if (b <= a && b <= c)
    return b;
  return c;
}

unsigned long concat(unsigned long a, unsigned long b) {
  size_t max_length_unsigned_long = 20;
  char result[max_length_unsigned_long * 2 + 1];
  snprintf(result, max_length_unsigned_long * 2, "%lu%lu", a, b);
  return strtoul(result, NULL, 10);
}

unsigned long lower_bound(node n, line input_line, bool allow_concat) {
  unsigned long result = n.acc;
  for (size_t i = n.depth; i < input_line.operands_len; i++) {
    unsigned long mult_result = result * input_line.operands[i];
    unsigned long sum_result = result + input_line.operands[i];
    unsigned long concat_result =
        allow_concat ? concat(result, input_line.operands[i]) : sum_result;
    result = min(mult_result, sum_result, concat_result);
  }
  return result;
}

unsigned long upper_bound(node n, line input_line, bool allow_concat) {
  unsigned long result = n.acc;
  for (size_t i = n.depth; i < input_line.operands_len; i++) {
    unsigned long mult_result = result * input_line.operands[i];
    unsigned long sum_result = result + input_line.operands[i];
    unsigned long concat_result =
        allow_concat ? concat(result, input_line.operands[i]) : sum_result;
    result = max(mult_result, sum_result, concat_result);
  }
  return result;
}

bool leads_to_solution(node n, line input_line, bool allow_concat) {
  if (n.depth == input_line.operands_len)
    return n.acc == input_line.expected;

  if (lower_bound(n, input_line, allow_concat) > input_line.expected)
    return false;

  if (upper_bound(n, input_line, allow_concat) < input_line.expected)
    return false;

  bool sum_solution = leads_to_solution(
      (node){
          .acc = n.acc + input_line.operands[n.depth],
          .depth = n.depth + 1,
      },
      input_line, allow_concat);

  if (sum_solution)
    return true;

  bool mult_solution = leads_to_solution(
      (node){
          .acc = n.acc * input_line.operands[n.depth],
          .depth = n.depth + 1,
      },
      input_line, allow_concat);

  if (mult_solution)
    return true;

  if (allow_concat) {
    bool concat_solution = leads_to_solution(
        (node){
            .acc = concat(n.acc, input_line.operands[n.depth]),
            .depth = n.depth + 1,
        },
        input_line, allow_concat);
    if (concat_solution)
      return true;
  }
  return false;
}

bool is_valid(line input_line, bool allow_concat) {
  node start_node = {.acc = input_line.operands[0], .depth = 1};
  return leads_to_solution(start_node, input_line, allow_concat);
}

unsigned long part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  unsigned long sum = 0;
  for (size_t i = 0; i < input.len; i++) {
    if (is_valid(input.lines[i], false)) {
      sum += input.lines[i].expected;
    }
  }
  return sum;
}

unsigned long part2(char *input_path) {
  parsed_input input = parse_input(input_path);
  unsigned long sum = 0;
  for (size_t i = 0; i < input.len; i++) {
    if (is_valid(input.lines[i], true)) {
      sum += input.lines[i].expected;
    }
  }
  return sum;
}
