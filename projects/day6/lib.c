#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct position {
  int x;
  int y;
} position;

typedef struct guard_state {
  position pos;
  int x_step;
  int y_step;
} guard_state;

guard_state create_guard(parsed_input input) {
  char *guard_formats = "^>v<";
  for (size_t i = 0; i < strlen(guard_formats); i++) {
    char ch = guard_formats[i];
    for (size_t y = 0; y < input.height; y++) {
      char *ptr = strchr(input.rows[y], ch);
      if (ptr != NULL) {
        int x_step;
        int y_step;
        switch (ch) {
        case '^': {
          x_step = 0;
          y_step = -1;
          break;
        }
        case '>': {
          x_step = 1;
          y_step = 0;
          break;
        }
        case 'v': {
          x_step = 0;
          y_step = 1;
          break;
        }
        case '<': {
          x_step = -1;
          y_step = 0;
        }
        };
        return (guard_state){
            .pos = {.x = (int)(ptr - input.rows[y]), .y = (int)y},
            .x_step = x_step,
            .y_step = y_step,
        };
      }
    }
  }
  fprintf(stderr, "failed to find guard");
  exit(1);
}

void print_state(parsed_input input, guard_state guard) {
  printf("\033[2J\033[1;1H"); // clear terminal
  for (size_t y = 0; y < input.height; y++) {
    for (size_t x = 0; x < input.width; x++) {
      if ((int)x == guard.pos.x && (int)y == guard.pos.y) {
        char guard_ch;
        if (guard.x_step == 0) {
          guard_ch = guard.y_step == 1 ? 'v' : '^';
        } else {
          guard_ch = guard.x_step == 1 ? '>' : '<';
        }
        printf("%c", guard_ch);
      } else {
        printf("%c", input.rows[y][x]);
      }
    }
    printf("\n");
  }
  printf("\n");
}

position next_position(guard_state *guard) {
  return (position){
      .x = guard->pos.x + guard->x_step,
      .y = guard->pos.y + guard->y_step,
  };
}

bool within_bounds(position pos, parsed_input input) {
  return pos.x >= 0 && pos.x < (int)input.width && pos.y >= 0 &&
         pos.y < (int)input.height;
}

char char_at_position(position pos, parsed_input input) {
  return input.rows[pos.y][pos.x];
}

void rotate(guard_state *guard) {
  int prev_x_step = guard->x_step;
  int prev_y_step = guard->y_step;
  guard->x_step = -prev_y_step;
  guard->y_step = prev_x_step;
}

void advance_guard(guard_state *guard, parsed_input input) {
  if (within_bounds(next_position(guard), input) &&
      char_at_position(next_position(guard), input) == '#') {
    rotate(guard);
  } else {
    guard->pos.x += guard->x_step;
    guard->pos.y += guard->y_step;
  }
}

bool equals(guard_state a, guard_state b) {
  return a.pos.x == b.pos.x && a.pos.y == b.pos.y && a.x_step == b.x_step &&
         a.y_step == b.y_step;
}

bool has_cycle(guard_state tortoise, parsed_input input) {
  guard_state hare = tortoise;

  while (true) {
    if (!within_bounds(hare.pos, input))
      return false;
    advance_guard(&tortoise, input);
    advance_guard(&hare, input);
    advance_guard(&hare, input);
    if (equals(tortoise, hare))
      return true;
  }
}

bool contains(position pos, position *pos_arr, size_t pos_arr_len) {
  for (size_t i = 0; i < pos_arr_len; i++) {
    if (pos_arr[i].x == pos.x && pos_arr[i].y == pos.y) {
      return true;
    }
  }
  return false;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  guard_state guard = create_guard(input);

  while (within_bounds(guard.pos, input)) {
    input.rows[guard.pos.y][guard.pos.x] = 'X';
    advance_guard(&guard, input);
  }

  int count = 0;
  for (size_t y = 0; y < input.height; y++) {
    for (size_t x = 0; x < input.width; x++) {
      if (input.rows[y][x] == 'X') {
        count++;
      }
    }
  }
  return count;
}

int part2(char *input_path) {
  parsed_input input = parse_input(input_path);
  guard_state guard = create_guard(input);

  position *obstacle_success_positions =
      malloc(sizeof(position) * input.height * input.width);
  size_t obstacle_success_positions_len = 0;

  position next;
  while (next = next_position(&guard), within_bounds(next, input)) {
    if (input.rows[next.y][next.x] != '#' &&
        !contains(next, obstacle_success_positions,
                  obstacle_success_positions_len)) {
      input.rows[next.y][next.x] = '#';
      // TODO - should only test each position once (or dedupe at the end)
      if (has_cycle(guard, input)) {
        obstacle_success_positions[obstacle_success_positions_len++] = next;
      }
      // clear obstacle
      input.rows[next.y][next.x] = '.';
    }
    advance_guard(&guard, input);
  }

  return (int)obstacle_success_positions_len;
}
