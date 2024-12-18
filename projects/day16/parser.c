#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>
#include <string.h>

parsed_input parse_input(char *input_path) {
  FileLines filelines = read_file_lines(input_path);

  return (parsed_input){
      .map_height = (int)filelines.line_count,
      .map_width = (int)strlen(filelines.lines[0]) - 1,
      .map = filelines.lines,
  };
}
