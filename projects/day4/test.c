#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 4\n");
  assert(part1("./inputs/example_input.txt") == 18);
  assert(part1("./inputs/input.txt") == 2613);
  assert(part2("./inputs/example_input.txt") == 9);
  assert(part2("./inputs/input.txt") == 1905);
}
