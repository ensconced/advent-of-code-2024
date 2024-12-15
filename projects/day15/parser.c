#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>
#include <string.h>

parsed_input parse_input(char *input_path) {
  FileLines filelines = read_file_lines(input_path);

  size_t map_idx = 0;
  while (strlen(filelines.lines[map_idx]) > 1) {
    map_idx++;
  }

  size_t movements_size = 0;
  size_t mvmt_start_line = map_idx + 1;
  for (size_t mvmt_idx = mvmt_start_line; mvmt_idx < filelines.line_count;
       mvmt_idx++) {
    movements_size += strlen(filelines.lines[mvmt_idx]);
  }

  char *movements = malloc(sizeof(char) * movements_size);
  size_t movements_len = 0;
  for (size_t mvmt_idx = mvmt_start_line; mvmt_idx < filelines.line_count;
       mvmt_idx++) {
    for (size_t i = 0; i < strlen(filelines.lines[mvmt_idx]); i++) {
      char mvmt = filelines.lines[mvmt_idx][i];
      if (mvmt != '\n') {
        movements[movements_len++] = mvmt;
      }
    }
  }

  return (parsed_input){
      .map = filelines.lines,
      .map_width = strlen(filelines.lines[0]) - 1,
      .map_height = mvmt_start_line - 1,
      .movements = movements,
      .movements_len = movements_len,
  };
}
