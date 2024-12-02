#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdlib.h>

bool interval_is_safe(unsigned long prev, unsigned long curr, bool increasing) {
  int a = (int)prev;
  int b = (int)curr;
  int diff = increasing ? b - a : a - b;
  return (diff <= 3 && diff >= 1);
}

bool part1_line_is_safe(parsed_line line) {
  if (line.length < 2)
    return true;

  bool increasing = (int)line.numbers[1] - (int)line.numbers[0] > 0;

  for (size_t i = 1; i < line.length; i++) {
    if (!interval_is_safe(line.numbers[i - 1], line.numbers[i], increasing)) {
      return false;
    }
  }
  return true;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);

  int safe_lines = 0;
  for (size_t i = 0; i < input.length; i++) {
    parsed_line line = input.lines[i];
    if (part1_line_is_safe(line)) {
      safe_lines++;
    }
  }

  return safe_lines;
}

bool part2_make_interval_safe(size_t *j, parsed_line line, bool increasing,
                              int *dampeners) {
  unsigned long prev = line.numbers[*j - 1];
  unsigned long curr = line.numbers[*j];

  if (interval_is_safe(prev, curr, increasing))
    return true;

  if (*dampeners > 0) {
    // if we're looking at the first or last interval, it's always fixable by
    // dropping the first or last element
    if (*j == 1 || *j == line.length - 1) {
      (*dampeners)--;
      return true;
    }

    // can we fix it by skipping prev?
    unsigned long prevprev = line.numbers[*j - 2];
    if (interval_is_safe(prevprev, curr, increasing)) {
      (*dampeners)--;
      return true;
    }

    // can we fix it by skipping curr?
    unsigned long next = line.numbers[*j + 1];
    if (interval_is_safe(prev, next, increasing)) {
      (*dampeners)--;
      (*j)++;
      return true;
    }
  }

  return false;
}

bool part2_line_is_safe(parsed_line line, bool increasing) {
  if (line.length < 2)
    return true;

  int dampeners = 1;
  for (size_t j = 1; j < line.length; j++) {
    if (!part2_make_interval_safe(&j, line, increasing, &dampeners))
      return false;
  }
  return true;
}

int part2(char *input_path) {
  parsed_input input = parse_input(input_path);

  int safe_lines = 0;
  for (size_t i = 0; i < input.length; i++) {
    parsed_line line = input.lines[i];
    if (part2_line_is_safe(line, true) || part2_line_is_safe(line, false)) {
      safe_lines++;
    }
  }

  return safe_lines;
}
