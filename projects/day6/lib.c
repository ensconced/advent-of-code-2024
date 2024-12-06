#include "./lib.h"
#include "./parser.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct guard_state {
  int x;
  int y;
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
            .x = (int)(ptr - input.rows[y]),
            .y = (int)y,
            .x_step = x_step,
            .y_step = y_step,
        };
      }
    }
  }
  fprintf(stderr, "failed to find guard");
  exit(1);
}

void print_state(parsed_input input) {
  printf("\033[2J\033[1;1H");
  for (size_t y = 0; y < input.height; y++) {
    for (size_t x = 0; x < input.width; x++) {
      printf("%c", input.rows[y][x]);
    }
    printf("\n");
  }
  printf("\n");
  sleep(0.5);
}

void advance_guard(guard_state *guard, parsed_input input) {
  while (guard->y + guard->y_step >= 0 &&
         guard->y + guard->y_step < (int)input.height &&
         guard->x + guard->x_step >= 0 &&
         guard->x + guard->x_step < (int)input.width &&
         input.rows[guard->y + guard->y_step][guard->x + guard->x_step] ==
             '#') {
    int prev_x_step = guard->x_step;
    int prev_y_step = guard->y_step;
    guard->x_step = -prev_y_step;
    guard->y_step = prev_x_step;
  }
  guard->x += guard->x_step;
  guard->y += guard->y_step;
}

int part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  guard_state guard = create_guard(input);

  while (guard.x >= 0 && guard.x < (int)input.width && guard.y >= 0 &&
         guard.y < (int)input.height) {
    input.rows[guard.y][guard.x] = 'X';
    print_state(input);
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
