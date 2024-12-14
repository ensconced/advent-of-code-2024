#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>

vec take_button(char *line) {
  char **ptr = &line;
  take_string("Button ", ptr);
  maybe_take_from_charset(ptr, "AB");
  take_string(": X+", ptr);
  unsigned long x = take_number(ptr);
  take_string(", Y+", ptr);
  unsigned long y = take_number(ptr);
  return (vec){.x = from_unsigned_long(x), .y = from_unsigned_long(y)};
}

vec take_prize(char *line) {
  char **ptr = &line;
  take_string("Prize: X=", ptr);
  unsigned long x = take_number(ptr);
  take_string(", Y=", ptr);
  unsigned long y = take_number(ptr);
  return (vec){.x = from_unsigned_long(x), .y = from_unsigned_long(y)};
}

game take_game(size_t line_idx, FileLines filelines) {
  return (game){
      .a = take_button(filelines.lines[line_idx]),
      .b = take_button(filelines.lines[line_idx + 1]),
      .prize = take_prize(filelines.lines[line_idx + 2]),
  };
}

parsed_input parse_input(char *input_path) {
  FileLines filelines = read_file_lines(input_path);

  size_t games_capacity = (filelines.line_count + 1) / 4;
  parsed_input result = {
      .games = malloc(sizeof(game) * games_capacity),
  };

  for (size_t line_idx = 0; line_idx < filelines.line_count; line_idx += 4) {
    result.games[result.game_len++] = take_game(line_idx, filelines);
  }
  return result;
}
