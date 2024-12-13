#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 11\n");
  assert(part1("./inputs/example_input.txt") == 55312);
  assert(part1("./inputs/input.txt") == 199986);
  assert(part2("./inputs/input.txt") == 236804088748754);
}
