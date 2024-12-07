#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
  usleep(1000 * 50);
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
      (char_at_position(next_position(guard), input) == '#' ||
       char_at_position(next_position(guard), input) == 'O')) {
    rotate(guard);
  } else {
    guard->pos.x += guard->x_step;
    guard->pos.y += guard->y_step;
  }
}

bool position_equals(position a, position b) {
  return a.x == b.x && a.y == b.y;
}

bool guard_equals(guard_state a, guard_state b) {
  return position_equals(a.pos, b.pos) && a.x_step == b.x_step &&
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
    if (guard_equals(tortoise, hare))
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

void trace_path(parsed_input input, guard_state guard) {
  while (within_bounds(guard.pos, input)) {
    input.rows[guard.pos.y][guard.pos.x] = 'X';
    advance_guard(&guard, input);
  }
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  guard_state guard = create_guard(input);
  trace_path(input, guard);

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
  guard_state initial_guard = create_guard(input);
  trace_path(input, initial_guard);

  int cycles_created = 0;
  for (size_t y = 0; y < input.height; y++) {
    for (size_t x = 0; x < input.width; x++) {
      position pos = {.x = (int)x, .y = (int)y};
      char ch = char_at_position(pos, input);
      if (ch == 'X' && !position_equals(pos, initial_guard.pos)) {
        guard_state fresh_guard = initial_guard;
        // create obstacle
        input.rows[pos.y][pos.x] = 'O';
        if (has_cycle(fresh_guard, input)) {
          cycles_created++;
        }
        // clear obstacle
        input.rows[pos.y][pos.x] = '.';
      }
    }
  }

  return cycles_created;
}
