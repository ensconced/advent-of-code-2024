#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>

#define MAX_UPDATE_LEN 64

parsed_input parse_input(char *input_path) {
  FileLines filelines = read_file_lines(input_path);
  ordered_pair *pairs = malloc(sizeof(ordered_pair) * filelines.line_count);
  size_t pairs_len = 0;
  update *updates = malloc(sizeof(update) * filelines.line_count);
  size_t updates_len = 0;

  size_t i = 0;
  while (true) {
    char *line = filelines.lines[i++];
    if (line[0] == '\n') {
      break;
    }
    unsigned long before = take_number(&line);
    take_string("|", &line);
    unsigned long after = take_number(&line);
    pairs[pairs_len++] = (ordered_pair){.before = before, .after = after};
  }
  while (i < filelines.line_count) {
    char *line = filelines.lines[i++];
    unsigned long *update_buffer =
        malloc(sizeof(unsigned long) * MAX_UPDATE_LEN);
    size_t update_len;
    take_separated_numbers(&line, ",", update_buffer, &update_len,
                           MAX_UPDATE_LEN);
    updates[updates_len++] =
        (update){.values = update_buffer, .len = update_len};
  }

  return (parsed_input){
      .pairs = pairs,
      .pairs_len = pairs_len,
      .updates = updates,
      .updates_len = updates_len,
  };
}
