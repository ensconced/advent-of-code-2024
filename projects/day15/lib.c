#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

typedef struct vec {
  int x;
  int y;
} vec;

bool make_space(vec pos, vec mvmt, parsed_input input) {
  char space = input.map[pos.y][pos.x];
  if (space == '.')
    return true;

  vec new_pos = (vec){.x = pos.x + mvmt.x, .y = pos.y + mvmt.y};
  bool can_move_small_box = space == 'O' && make_space(new_pos, mvmt, input);
  if (can_move_small_box) {
    input.map[pos.y][pos.x] = '.';
    input.map[new_pos.y][new_pos.x] = 'O';
    return true;
  }

  // bool can_move_wide_box_lhs = space == '[' && make_space(new_pos, mvmt,
  // input); bool can_move_wide_box_rhs =
  //     space == ']' &&
  //     make_space((vec){.x = new_pos.x - 1, .y = new_pos.y}, mvmt, input);

  // if ((space == 'O' && move_box(new_pos, mvmt, input))) {
  //   input.map[pos.y][pos.x] = '.';
  //   input.map[new_pos.y][new_pos.x] = 'O';
  //   return true;
  // }
  return false;
}

const int steps[4][3] = {
    {(int)'<', -1, 0}, {(int)'>', 1, 0}, {(int)'^', 0, -1}, {(int)'v', 0, 1}};

void move_robot(vec *robot_position, parsed_input input, char mvmt) {
  int step_x = 0, step_y = 0;
  for (size_t i = 0; i < 4; i++) {
    if ((int)mvmt == steps[i][0]) {
      step_x = steps[i][1];
      step_y = steps[i][2];
      break;
    }
  }

  vec new_pos = (vec){
      .x = robot_position->x + step_x,
      .y = robot_position->y + step_y,
  };

  vec mvmt_vec = {.x = step_x, .y = step_y};
  if (make_space(new_pos, mvmt_vec, input)) {
    *robot_position = new_pos;
  }
}

vec find_robot(parsed_input input) {
  for (int y = 1; y < (int)input.map_height - 1; y++) {
    for (int x = 1; x < (int)input.map_width - 1; x++) {
      if (input.map[y][x] == '@') {
        return (vec){.x = x, .y = y};
      }
    }
  }
  fprintf(stderr, "failed to find robot\n");
  exit(1);
}

void draw_map(parsed_input input, vec robot) {
  printf("\033[2J\033[1;1H"); // clear terminal

  for (int y = 0; y < (int)input.map_height; y++) {
    for (int x = 0; x < (int)input.map_width; x++) {
      if (robot.x == x && robot.y == y) {
        printf("@");
      } else {
        printf("%c", input.map[y][x]);
      }
    }
    printf("\n");
  }
}

int sum_box_coords(parsed_input input) {
  int result = 0;
  for (int y = 0; y < (int)input.map_height; y++) {
    for (int x = 0; x < (int)input.map_width; x++) {
      if (input.map[y][x] == 'O') {
        result += 100 * y + x;
      }
    }
  }
  return result;
}

void widen_map(parsed_input *input) {
  char **new_map = malloc(sizeof(char *) * input->map_height);
  for (size_t y = 0; y < input->map_height; y++) {
    char *row = malloc(sizeof(char) * input->map_width * 2);
    for (size_t x = 0; x < input->map_width; x++) {
      switch (input->map[y][x]) {
      case '#': {
        row[x * 2] = '#';
        row[x * 2 + 1] = '#';
        break;
      }
      case '0': {
        row[x * 2] = '[';
        row[x * 2 + 1] = ']';
        break;
      }
      case '.': {
        row[x * 2] = '.';
        row[x * 2 + 1] = '.';
        break;
      }
      case '@': {
        row[x * 2] = '@';
        row[x * 2 + 1] = '.';
        break;
      }
      }
    }
  }
  input->map = new_map;
  input->map_width = input->map_width * 2;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  vec robot = find_robot(input);
  input.map[robot.y][robot.x] = '.';
  for (size_t i = 0; i < input.movements_len; i++) {
    move_robot(&robot, input, input.movements[i]);
  }
  return sum_box_coords(input);
}

int part2(char *input_path) {
  parsed_input input = parse_input(input_path);
  widen_map(&input);

  vec robot = find_robot(input);
  input.map[robot.y][robot.x] = '.';
  for (size_t i = 0; i < input.movements_len; i++) {
    // draw_map(input, robot);
    // printf("\ni: %lu, next mvmt: %c\n", i, input.movements[i]);
    // usleep(1000 * 10);
    move_robot(&robot, input, input.movements[i]);
  }
  return sum_box_coords(input);
}
