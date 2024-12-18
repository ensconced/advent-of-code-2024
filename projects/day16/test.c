#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 16\n");
  assert(part1("./inputs/example_input.txt") == 7036);
  assert(part1("./inputs/input.txt") == 75416);
  assert(part2("./inputs/example_input.txt") == 45);
  assert(part2("./inputs/input.txt") == 476);
}
