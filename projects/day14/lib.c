#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void move_robot(robot *bot, int height, int width) {
  bot->position.x = bot->position.x + bot->velocity.x;
  bot->position.y = bot->position.y + bot->velocity.y;
  while (bot->position.x < 0) {
    bot->position.x += width;
  }
  while (bot->position.x >= width) {
    bot->position.x -= width;
  }
  while (bot->position.y < 0) {
    bot->position.y += height;
  }
  while (bot->position.y >= height) {
    bot->position.y -= height;
  }
}

int compare_robots(const void *a, const void *b) {
  robot *robot_a = (robot *)a;
  robot *robot_b = (robot *)b;
  if (robot_a->position.y != robot_b->position.y)
    return robot_a->position.y - robot_b->position.y;
  return robot_a->position.x - robot_b->position.x;
}

int part1(char *input_path, int height, int width) {
  parsed_input input = parse_input(input_path);
  for (size_t tick = 0; tick < 100; tick++) {
    for (size_t i = 0; i < input.len; i++) {
      move_robot(&input.robots[i], height, width);
    }
  }

  int top_left = 0;
  int top_right = 0;
  int bottom_left = 0;
  int bottom_right = 0;
  for (size_t i = 0; i < input.len; i++) {
    robot bot = input.robots[i];
    if (bot.position.x < width / 2) {
      if (bot.position.y < height / 2) {
        top_left++;
      } else if (bot.position.y > height / 2) {
        bottom_left++;
      }
    } else if (bot.position.x > width / 2) {
      if (bot.position.y < height / 2) {
        top_right++;
      } else if (bot.position.y > height / 2) {
        bottom_right++;
      }
    }
  }
  return top_left * top_right * bottom_left * bottom_right;
}

bool has_tree(char *scene, int height, int width) {
  for (int y = 0; y < height; y++) {
    int score = 0;
    for (int x = 0; x < width; x++) {
      if (scene[y * width + x] == 'X') {
        score++;
        if (score == 10)
          return true;
      } else {
        score = 0;
      }
    }
  }
  return false;
}

void draw_scene(char *scene, int height, int width) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      printf("%c", scene[y * width + x]);
    }
    printf("\n");
  }
}

int part2(char *input_path, int height, int width) {
  parsed_input input = parse_input(input_path);
  char *scene = malloc(sizeof(char) * (size_t)(height * width));
  int seconds = 0;
  while (true) {
    memset(scene, (int)'.', (size_t)(height * width));
    for (size_t i = 0; i < input.len; i++) {
      robot bot = input.robots[i];
      scene[bot.position.y * width + bot.position.x] = 'X';
    }

    if (has_tree(scene, height, width)) {
      // draw_scene(scene, height, width);
      return seconds;
    }

    for (size_t i = 0; i < input.len; i++) {
      move_robot(&input.robots[i], height, width);
    }
    seconds++;
  }
}
