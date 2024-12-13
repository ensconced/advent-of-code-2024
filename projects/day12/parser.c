
#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "./lib.h"
#include <string.h>

parsed_input parse_input(char *input_path) {
  FileLines filelines = read_file_lines(input_path);
  return (parsed_input){
      .height = (int)filelines.line_count,
      .width = (int)(strlen(filelines.lines[0]) - 1),
      .rows = filelines.lines,
  };
}
