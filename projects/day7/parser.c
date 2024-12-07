#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>

#define MAX_OPERANDS 64

parsed_input parse_input(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  parsed_input result = {.lines = malloc(sizeof(line) * file_lines.line_count)};
  for (size_t i = 0; i < file_lines.line_count; i++) {
    char *file_line = file_lines.lines[i];
    unsigned long expected = take_number(&file_line);
    take_string(": ", &file_line);
    unsigned long *numbers = malloc(sizeof(unsigned long) * MAX_OPERANDS);
    size_t numbers_len = 0;
    take_separated_numbers(&file_line, " ", numbers, &numbers_len,
                           MAX_OPERANDS);
    result.lines[result.len++] = (line){
        .expected = expected,
        .operands = numbers,
        .operands_len = numbers_len,
    };
  }
  return result;
}
