#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 6\n");
  assert(part1("./inputs/example_input.txt") == 41);
  assert(part1("./inputs/input.txt") == 5534);
  assert(part2("./inputs/example_input.txt") == 6);
  assert(part2("./inputs/input.txt") == 2262);
}
