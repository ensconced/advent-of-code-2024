#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 14\n");
  assert(part1("./inputs/example_input.txt", 7, 11) == 12);
  assert(part1("./inputs/input.txt", 103, 101) == 225521010);
  assert(part2("./inputs/input.txt", 103, 101) == 7774);
}
