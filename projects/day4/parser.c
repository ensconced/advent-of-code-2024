#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 64

char *without_whitespace(char *str) {
  char *copy = malloc(sizeof(char) * (strlen(str) + 1));
  for (size_t i = 0; i <= strlen(str); i++) {
    copy[i] = str[i];
  }
  return copy;
}

parsed_input parse_input(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  char **parsed_lines = malloc(sizeof(char *) * file_lines.line_count);
  for (size_t i = 0; i < file_lines.line_count; i++) {
    char *line = file_lines.lines[i];
    parsed_lines[i] = without_whitespace(line);
  }
  return (parsed_input){
      .lines = parsed_lines,
      .height = file_lines.line_count,
      .width = strlen(parsed_lines[0]),
  };
}
