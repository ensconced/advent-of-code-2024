#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct vec {
  int x;
  int y;
} vec;

typedef struct antenna {
  char ch;
  vec pos;
} antenna;

typedef struct antenna_list {
  antenna *buffer;
  size_t len;
} antenna_list;

typedef struct grouped_antennas {
  antenna_list *buffer;
  size_t len;
} grouped_antennas;

typedef struct antenna_pair {
  antenna a;
  antenna b;
} antenna_pair;

typedef struct antenna_pair_list {
  antenna_pair *buffer;
  size_t len;
} antenna_pair_list;

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
            .pos = {.x = (int)x, .y = (int)y},
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

bool is_within_grid(vec pos, parsed_input input) {
  return pos.x >= 0 && pos.x < (int)input.width && pos.y >= 0 &&
         pos.y < (int)input.height;
}

vec translate(vec v, vec d) {
  return (vec){
      .x = v.x + d.x,
      .y = v.y + d.y,
  };
}

vec scale(vec v, int scalar) {
  return (vec){
      .x = v.x * scalar,
      .y = v.y * scalar,
  };
}

int greatest_common_divisor(int a, int b) {
  int bigger = a > b ? a : b;
  int smaller = bigger == a ? b : a;
  if (smaller == 0)
    return bigger;
  return greatest_common_divisor(bigger % smaller, smaller);
}

vec simplify(vec v) {
  int gcd = greatest_common_divisor(abs(v.x), abs(v.y));
  return (vec){.x = v.x / gcd, .y = v.y / gcd};
}

void draw_part1_antinodes(antenna a, antenna b, parsed_input input) {
  vec step = {.x = b.pos.x - a.pos.x, .y = b.pos.y - a.pos.y};
  vec antinode_a = translate(a.pos, scale(step, -1));
  vec antinode_b = translate(b.pos, step);

  if (is_within_grid(antinode_a, input)) {
    input.rows[antinode_a.y][antinode_a.x] = '#';
  }
  if (is_within_grid(antinode_b, input)) {
    input.rows[antinode_b.y][antinode_b.x] = '#';
  }
}

void draw_part2_antinodes(antenna a, antenna b, parsed_input input) {
  vec step = {.x = b.pos.x - a.pos.x, .y = b.pos.y - a.pos.y};
  vec simplified_step = simplify(step);
  vec reversed_simplified_step = scale(simplified_step, -1);
  for (vec pos = a.pos; is_within_grid(pos, input);
       pos = translate(pos, simplified_step)) {
    input.rows[pos.y][pos.x] = '#';
  }
  for (vec pos = a.pos; is_within_grid(pos, input);
       pos = translate(pos, reversed_simplified_step)) {
    input.rows[pos.y][pos.x] = '#';
  }
}

antenna_pair_list antenna_pairs(parsed_input input) {
  antenna_list antennas = locate_antennas(input);
  grouped_antennas grouped_ants = group_antennas(antennas);
  size_t pair_count = (antennas.len * antennas.len) / 2 - antennas.len;
  antenna_pair_list pair_list = {.buffer =
                                     malloc(sizeof(antenna_pair) * pair_count)};

  for (size_t i = 0; i < grouped_ants.len; i++) {
    antenna_list group = grouped_ants.buffer[i];
    for (size_t j = 0; j < group.len; j++) {
      for (size_t k = j + 1; k < group.len; k++) {
        pair_list.buffer[pair_list.len++] = (antenna_pair){
            .a = group.buffer[j],
            .b = group.buffer[k],
        };
      }
    }
  }

  return pair_list;
}

int count_antinodes(parsed_input input) {
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

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  antenna_pair_list pairs = antenna_pairs(input);

  for (size_t i = 0; i < pairs.len; i++) {
    antenna_pair pair = pairs.buffer[i];
    draw_part1_antinodes(pair.a, pair.b, input);
  }

  return count_antinodes(input);
}

int part2(char *input_path) {
  parsed_input input = parse_input(input_path);
  antenna_pair_list pairs = antenna_pairs(input);

  for (size_t i = 0; i < pairs.len; i++) {
    antenna_pair pair = pairs.buffer[i];
    draw_part2_antinodes(pair.a, pair.b, input);
  }

  return count_antinodes(input);
}
