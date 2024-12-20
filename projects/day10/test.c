#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 10\n");
  assert(part1("./inputs/example_input.txt") == 36);
  assert(part1("./inputs/input.txt") == 468);
  assert(part2("./inputs/example_input.txt") == 81);
  assert(part2("./inputs/input.txt") == 966);
}
