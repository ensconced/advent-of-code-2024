#include "./lib.h"
#include "./math.h"
#include "./parser.h"
#include <stdio.h>

#define CACHE_HEIGHT 75
#define CACHE_WIDTH 4028

unsigned long long digit_count(unsigned long long num) {
  return (unsigned long long)log10((double)num) + 1;
}

size_t size_after_blinks(unsigned long long stone, size_t blink_count,
                         unsigned long long *cache) {
  if (blink_count == 0)
    return 1;

  if (stone < CACHE_WIDTH && blink_count < CACHE_HEIGHT) {
    unsigned long long cached_result = cache[blink_count * CACHE_WIDTH + stone];
    if (cached_result)
      return cached_result;
  }

  size_t result;

  if (stone == 0) {
    result = size_after_blinks(1, blink_count - 1, cache);
  } else {
    unsigned long long digits = digit_count(stone);
    if (digits % 2 == 0) {
      unsigned long long power =
          (unsigned long long)pow(10, (double)(digits / 2));
      unsigned long long lhs = stone / power;
      unsigned long long rhs = stone % power;
      result = size_after_blinks(lhs, blink_count - 1, cache) +
               size_after_blinks(rhs, blink_count - 1, cache);
    } else {
      result = size_after_blinks(stone * 2024, blink_count - 1, cache);
    }
  }

  if (stone < CACHE_WIDTH && blink_count < CACHE_HEIGHT) {
    cache[blink_count * CACHE_WIDTH + stone] = result;
  }
  return result;
}

unsigned long long *create_cache() {
  unsigned long long *cache =
      calloc(CACHE_WIDTH * CACHE_HEIGHT, sizeof(unsigned long long));
  return cache;
}

size_t part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  unsigned long long *cache = create_cache();
  size_t result = 0;
  for (size_t i = 0; i < input.len; i++) {
    result += size_after_blinks(input.numbers[i], 25, cache);
  }
  return result;
}

size_t part2(char *input_path) {
  parsed_input input = parse_input(input_path);
  unsigned long long *cache = create_cache();
  size_t result = 0;
  for (size_t i = 0; i < input.len; i++) {
    result += size_after_blinks(input.numbers[i], 75, cache);
  }
  return result;
}
