#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdlib.h>

bool diff_is_safe(unsigned long prev, unsigned long curr, bool increasing) {
  int b = (int)curr;
  int a = (int)prev;
  int diff = increasing ? b - a : a - b;
  return (diff <= 3 && diff > 0);
}

bool line_is_safe(parsed_line line, bool dampener) {
  if (line.length < 2)
    return true;

  bool increasing = line.numbers[1] > line.numbers[0];
  for (size_t j = 1; j < line.length; j++) {
    unsigned long prev = line.numbers[j - 1];
    unsigned long curr = line.numbers[j];
    if (!diff_is_safe(prev, curr, increasing))
      return false;
  }
  return true;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);

  int safe_lines = 0;
  for (size_t i = 0; i < input.length; i++) {
    parsed_line line = input.lines[i];
    if (line_is_safe(line, false)) {
      safe_lines++;
    }
  }

  return safe_lines;
}

int part2(char *input_path) {
  parsed_input input = parse_input(input_path);

  int safe_lines = 0;
  for (size_t i = 0; i < input.length; i++) {
    parsed_line line = input.lines[i];
    if (line_is_safe(line, true)) {
      safe_lines++;
    }
  }

  return safe_lines;
}
