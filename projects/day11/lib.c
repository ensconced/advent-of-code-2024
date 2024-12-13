#include "./lib.h"
#include "./math.h"
#include "./parser.h"
#include <stdio.h>

unsigned long long digit_count(unsigned long long num) {
  return (unsigned long long)log10((double)num) + 1;
}

size_t size_after_blinks(unsigned long long *stones, size_t stones_len,
                         size_t blink_count) {
  for (size_t i = 0; i < blink_count; i++) {
    unsigned long long *new_stones =
        malloc(sizeof(unsigned long long) * stones_len * 2);
    size_t new_stones_len = 0;
    for (size_t j = 0; j < stones_len; j++) {
      unsigned long long stone = stones[j];
      if (stone == 0) {
        new_stones[new_stones_len++] = 1;
      } else {
        unsigned long long digits = digit_count(stone);
        if (digits % 2 == 0) {
          unsigned long long power =
              (unsigned long long)pow(10, (double)(digits / 2));
          unsigned long long lhs = stone / power;
          unsigned long long rhs = stone % power;
          new_stones[new_stones_len++] = lhs;
          new_stones[new_stones_len++] = rhs;
        } else {
          new_stones[new_stones_len++] = stone * 2024;
        }
      }
    }
    stones = new_stones;
    stones_len = new_stones_len;
  }
  return stones_len;
}

size_t part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  return size_after_blinks(input.numbers, input.len, 25);
}
size_t part2(char *input_path) {
  parsed_input input = parse_input(input_path);
  return size_after_blinks(input.numbers, input.len, 75);
}
