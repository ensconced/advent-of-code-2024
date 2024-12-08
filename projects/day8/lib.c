#include "./lib.h"
#include "./parser.h"
#include <stdio.h>
#include <string.h>

typedef struct antenna {
  char ch;
  int x;
  int y;
} antenna;

typedef struct antenna_list {
  antenna *buffer;
  size_t len;
} antenna_list;

typedef struct grouped_antennas {
  antenna_list *buffer;
  size_t len;
} grouped_antennas;

antenna_list locate_antennas(parsed_input input) {
  antenna_list antennas = {
      .buffer = malloc(sizeof(antenna) * input.height * input.width),
  };
  for (size_t y = 0; y < input.height; y++) {
    for (size_t x = 0; x < input.width; x++) {
      char ch = input.rows[y][x];
      if (ch != '.') {
        antennas.buffer[antennas.len++] = (antenna){
            .ch = ch,
            .x = (int)x,
            .y = (int)y,
        };
      }
    }
  }
  return antennas;
}

int compare_antennas(const void *a, const void *b) {
  return ((antenna *)(a))->ch - ((antenna *)(b))->ch;
}

grouped_antennas group_antennas(antenna_list antennas) {
  qsort(antennas.buffer, antennas.len, sizeof(antenna), compare_antennas);

  grouped_antennas result = {
      .buffer = malloc(sizeof(antenna_list) * antennas.len),
  };
  char current_char;
  antenna_list current_group;
  for (size_t i = 0; i < antennas.len; i++) {
    antenna ant = antennas.buffer[i];
    if (i == 0 || ant.ch != current_char) {
      if (i != 0) {
        result.buffer[result.len++] = current_group;
      }
      current_group = (antenna_list){
          .buffer = malloc(sizeof(antenna) * antennas.len),
      };
      current_char = ant.ch;
    }
    current_group.buffer[current_group.len++] = ant;
  }
  if (current_group.len > 0) {
    result.buffer[result.len++] = current_group;
  }
  return result;
}

void draw_antinodes_for_antenna_pair(antenna a, antenna b, parsed_input input) {
  int x_step = b.x - a.x;
  int y_step = b.y - a.y;
  int antinode_b_x = b.x + x_step;
  int antinode_b_y = b.y + y_step;
  int antinode_a_x = a.x - x_step;
  int antinode_a_y = a.y - y_step;

  if (antinode_a_x >= 0 && antinode_a_x < (int)input.width &&
      antinode_a_y >= 0 && antinode_a_y < (int)input.height) {
    input.rows[antinode_a_y][antinode_a_x] = '#';
  }
  if (antinode_b_x >= 0 && antinode_b_x < (int)input.width &&
      antinode_b_y >= 0 && antinode_b_y < (int)input.height) {
    input.rows[antinode_b_y][antinode_b_x] = '#';
  }
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  antenna_list antennas = locate_antennas(input);
  grouped_antennas grouped_ants = group_antennas(antennas);
  for (size_t i = 0; i < grouped_ants.len; i++) {
    antenna_list group = grouped_ants.buffer[i];
    for (size_t j = 0; j < group.len; j++) {
      for (size_t k = 0; k < group.len; k++) {
        if (j != k) {
          draw_antinodes_for_antenna_pair(group.buffer[j], group.buffer[k],
                                          input);
        }
      }
    }
  }

  int result = 0;
  for (size_t y = 0; y < input.height; y++) {
    for (size_t x = 0; x < input.width; x++) {
      if (input.rows[y][x] == '#') {
        result++;
      }
    }
  }

  return result;
}
