#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>
#include <string.h>

parsed_input parse_input(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  return (parsed_input){
      .height = file_lines.line_count,
      // -1 for the newline at the end
      .width = strlen(file_lines.lines[0]) - 1,
      .rows = file_lines.lines,
  };
}
