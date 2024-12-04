#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void search_along_line(parsed_input input, char *search_string,
                       int *xmas_centre_hitcounts, int start_x, int start_y,
                       int step_x, int step_y) {
  size_t chars_matched = 0;

  int x = start_x;
  int y = start_y;
  while (x >= 0 && x < (int)input.width && y >= 0 && y < (int)input.height) {
    char ch = input.lines[y][x];

    if (ch == search_string[chars_matched]) {
      chars_matched++;
      if (chars_matched == strlen(search_string)) {
        // find position of the "A"
        size_t a_y = (size_t)(y - step_y);
        size_t a_x = (size_t)(x - step_x);
        xmas_centre_hitcounts[a_y * input.width + a_x]++;
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
}

void search_lines_from_side(parsed_input input, char *search_string,
                            int *xmas_centre_hitcounts, bool diagonals_only,
                            int start_x, int start_y, int side_x, int side_y) {
  for (int x = (int)start_x, y = (int)start_y;
       x >= 0 && x < (int)input.width && y >= 0 && y < (int)input.height;
       x += side_x, y += side_y) {
    if (!diagonals_only) {
      search_along_line(input, search_string, xmas_centre_hitcounts, x, y,
                        side_y, -side_x);
    }
    search_along_line(input, search_string, xmas_centre_hitcounts, x, y,
                      side_y - side_x, -side_x - side_y);
    // avoid double-counting corner diagonals
    if (x != start_x || y != start_y) {
      search_along_line(input, search_string, xmas_centre_hitcounts, x, y,
                        side_y + side_x, side_y - side_x);
    }
  }
}

int *get_hitcounts(parsed_input input, char *search_string,
                   bool diagonals_only) {
  int *xmas_centre_hitcounts = calloc(input.width * input.height, sizeof(int));

  search_lines_from_side(input, search_string, xmas_centre_hitcounts,
                         diagonals_only, 0, 0, 0, 1);
  search_lines_from_side(input, search_string, xmas_centre_hitcounts,
                         diagonals_only, 0, (int)input.height - 1, 1, 0);
  search_lines_from_side(input, search_string, xmas_centre_hitcounts,
                         diagonals_only, (int)input.width - 1,
                         (int)input.height - 1, 0, -1);
  search_lines_from_side(input, search_string, xmas_centre_hitcounts,
                         diagonals_only, (int)input.width - 1, 0, -1, 0);

  return xmas_centre_hitcounts;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  int *xmas_centre_hitcounts = get_hitcounts(input, "XMAS", false);
  int result = 0;
  for (size_t i = 0; i < input.width * input.height; i++) {
    result += xmas_centre_hitcounts[i];
  }
  return result;
}

int part2(char *input_path) {
  parsed_input input = parse_input(input_path);
  int *xmas_centre_hitcounts = get_hitcounts(input, "MAS", true);
  int result = 0;
  for (size_t i = 0; i < input.width * input.height; i++) {
    if (xmas_centre_hitcounts[i] > 1) {
      result++;
    }
  }
  return result;
}
