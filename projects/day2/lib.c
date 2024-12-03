#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

parsed_line copy_line(parsed_line line) {
  unsigned long *numbers = malloc(sizeof(unsigned long *) * line.length);
  for (size_t i = 0; i < line.length; i++) {
    numbers[i] = line.numbers[i];
  }
  return (parsed_line){
      .numbers = numbers,
      .length = line.length,
  };
}

void delete_at_index(parsed_line *line, size_t index) {
  for (size_t i = index; i < line->length - 1; i++) {
    line->numbers[i] = line->numbers[i + 1];
  }
  line->length--;
}

bool interval_is_safe(unsigned long prev, unsigned long curr, bool increasing) {
  int a = (int)prev;
  int b = (int)curr;
  int interval = increasing ? b - a : a - b;
  return (interval <= 3 && interval >= 1);
}

bool line_is_safe(parsed_line line, bool increasing) {
  if (line.length < 2)
    return true;

  for (size_t i = 1; i < line.length; i++) {
    unsigned long prev = line.numbers[i - 1];
    unsigned long curr = line.numbers[i];
    if (!interval_is_safe(prev, curr, increasing))
      return false;
  }
  return true;
}

bool line_is_safe_with_dampener(parsed_line line, bool increasing) {
  if (line_is_safe(line, increasing))
    return true;

  for (size_t i = 0; i < line.length; i++) {
    parsed_line copy = copy_line(line);
    delete_at_index(&copy, i);
    if (line_is_safe(copy, increasing))
      return true;
  }

  return false;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);

  int safe_lines = 0;
  for (size_t i = 0; i < input.length; i++) {
    parsed_line line = input.lines[i];
    if (line_is_safe(line, true) || line_is_safe(line, false)) {
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
    if (line_is_safe_with_dampener(line, true) ||
        line_is_safe_with_dampener(line, false)) {
      safe_lines++;
    }
  }

  return safe_lines;
}
