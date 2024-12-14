#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>

parsed_input parse_input(char *input_path) {
  FileLines filelines = read_file_lines(input_path);
  parsed_input result = {
      .robots = malloc(sizeof(robot) * filelines.line_count),
  };
  for (size_t i = 0; i < filelines.line_count; i++) {
    char *line = filelines.lines[i];
    char **ptr = &line;
    take_string("p=", ptr);
    vec position;
    position.x = (int)take_number(ptr);
    take_string(",", ptr);
    position.y = (int)take_number(ptr);
    take_string(" v=", ptr);

    vec velocity;
    velocity.x = (int)take_signed_number(ptr);
    take_string(",", ptr);
    velocity.y = (int)take_signed_number(ptr);

    result.robots[result.len++] =
        (robot){.position = position, .velocity = velocity};
  }
  return result;
}
