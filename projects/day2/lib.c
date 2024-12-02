#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool interval_is_safe(unsigned long prev, unsigned long curr, bool increasing) {
  int a = (int)prev;
  int b = (int)curr;
  int interval = increasing ? b - a : a - b;
  return (interval <= 3 && interval >= 1);
}

bool interval_can_be_made_safe_using_dampener(size_t i, parsed_line line,
                                              bool increasing) {
  unsigned long prev = line.numbers[i - 1];
  unsigned long curr = line.numbers[i];

  // first, check if the following interval is also dodgy. if so, our only
  // chance is to remove the node in the middle of the two dodgy intervals...

  // if not, we can remove either the node before the interval or the node
  // after the interval - whichever doesn't create another bad interval...

  // if we're looking at the first or last interval, it's always fixable by
  // dropping the first or last element.
  // if (*j == 1 || *j == line.length - 1) {
  //   (*dampeners)--;
  //   return true;
  // }

  // can we fix it by skipping prev?
  unsigned long prevprev = line.numbers[i - 2];
  if (interval_is_safe(prevprev, curr, increasing)) {
    return true;
  }

  // can we fix it by skipping curr?
  unsigned long next = line.numbers[i + 1];
  if (interval_is_safe(prev, next, increasing)) {
    (*j)++;
    return true;
  }

  return false;
}

bool line_is_safe(parsed_line line, bool increasing, bool can_use_dampener) {
  if (line.length < 2)
    return true;

  for (size_t i = 1; i < line.length; i++) {
    unsigned long prev = line.numbers[i - 1];
    unsigned long curr = line.numbers[i];

    if (interval_is_safe(prev, curr, increasing)) {
      continue;
    }

    if (can_use_dampener &&
        interval_can_be_made_safe_using_dampener(i, line, increasing)) {
      can_use_dampener = false;
      continue;
    }

    return false;
  }
  return true;
}

int count_safe_lines(char *input_path, bool can_use_dampener) {
  parsed_input input = parse_input(input_path);

  int safe_lines = 0;
  for (size_t i = 0; i < input.length; i++) {
    parsed_line line = input.lines[i];
    if (line_is_safe(line, true, can_use_dampener) ||
        line_is_safe(line, false, can_use_dampener)) {
      safe_lines++;
    }
  }

  return safe_lines;
}

int part1(char *input_path) { return count_safe_lines(input_path, false); }
int part2(char *input_path) { return count_safe_lines(input_path, true); }
