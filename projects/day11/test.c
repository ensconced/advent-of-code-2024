#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  assert(part1("./inputs/example_input.txt") == 55312);
  assert(part1("./inputs/input.txt") == 199986);
  // printf("part 2: %lu\n", part2("./inputs/input.txt"));
}
