#include "./lib.h"
#include "./parser.h"
#include <stdio.h>
#include <string.h>

int search_along_line(parsed_input input, int start_x, int start_y, int step_x,
                      int step_y) {
  int found = 0;
  char *search_string = "XMAS";
  size_t chars_matched = 0;

  int x = start_x;
  int y = start_y;
  while (x >= 0 && x < (int)input.width && y >= 0 && y < (int)input.height) {
    char ch = input.lines[y][x];

    if (ch == search_string[chars_matched]) {
      chars_matched++;
      if (chars_matched == strlen(search_string)) {
        found++;
        chars_matched = 0;
      }
    } else {
      chars_matched = 0;
      if (ch == search_string[0]) {
        chars_matched++;
      }
    }

    x += step_x;
    y += step_y;
  }

  return found;
}

int search_lines_from_side(parsed_input input, int start_x, int start_y,
                           int side_x, int side_y) {
  int result = 0;
  for (int x = (int)start_x, y = (int)start_y;
       x >= 0 && x < (int)input.width && y >= 0 && y < (int)input.height;
       x += side_x, y += side_y) {
    result += search_along_line(input, x, y, side_y, -side_x);
    result += search_along_line(input, x, y, side_y - side_x, -side_x - side_y);
    // avoid double-counting corner diagonals
    if (x != start_x || y != start_y) {
      result +=
          search_along_line(input, x, y, side_y + side_x, side_y - side_x);
    }
  }
  return result;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);

  int result = 0;
  result += search_lines_from_side(input, 0, 0, 0, 1);
  result += search_lines_from_side(input, 0, (int)input.height - 1, 1, 0);
  result += search_lines_from_side(input, (int)input.width - 1,
                                   (int)input.height - 1, 0, -1);
  result += search_lines_from_side(input, (int)input.width - 1, 0, -1, 0);
  return result;
}
