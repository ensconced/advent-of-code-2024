#include "./lib.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include <string.h>

int compute_corrupted_expression(char *input_path, bool part2) {
  char *input = read_text_file(input_path);
  char *ptr = input;
  int result = 0;

  bool multiply_enabled = true;
  while (ptr < input + strlen(input)) {
    if (part2 && maybe_take_string("do()", &ptr)) {
      multiply_enabled = true;
    } else if (part2 && maybe_take_string("don't()", &ptr)) {
      multiply_enabled = false;
    } else if (multiply_enabled && maybe_take_string("mul(", &ptr)) {
      unsigned long a, b;
      if (maybe_take_number(&ptr, &a) && maybe_take_string(",", &ptr) &&
          maybe_take_number(&ptr, &b) && maybe_take_string(")", &ptr)) {
        result += a * b;
      }
    } else {
      ptr++;
    }
  }

  return result;
}

int part1(char *input_path) {
  return compute_corrupted_expression(input_path, false);
}

int part2(char *input_path) {
  return compute_corrupted_expression(input_path, true);
}
