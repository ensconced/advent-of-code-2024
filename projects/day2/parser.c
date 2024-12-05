#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include <stdio.h>

#define MAX_LINE_LENGTH 128

parsed_input parse_input(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);

  parsed_line *parsed_lines =
      malloc(sizeof(parsed_line) * file_lines.line_count);

  for (size_t i = 0; i < file_lines.line_count; i++) {
    char *line = file_lines.lines[i];
    parsed_lines[i].numbers = malloc(sizeof(unsigned long) * MAX_LINE_LENGTH);
    take_separated_numbers(&line, " ", parsed_lines[i].numbers,
                           &parsed_lines[i].length, MAX_LINE_LENGTH);
  }

  return (parsed_input){
      .length = file_lines.line_count,
      .lines = parsed_lines,
  };
}
