#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>

#define MAX_LINE_LENGTH 128

void parse_input(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);

  for (size_t i = 0; i < file_lines.line_count; i++) {
    char *line = file_lines.lines[i];
    unsigned long buffer = malloc(sizeof(unsigned long) * MAX_LINE_LENGTH);
    take_whitespace_separated_numbers(&line, );
  }

  int **matrix = malloc(file_lines.line_count * sizeof(int *));

  int safe_lines = 0;

  free(matrix);
  return safe_lines;
}
