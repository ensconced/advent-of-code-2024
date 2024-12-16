#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 15\n");
  assert(part1("./inputs/small_example_input.txt") == 2028);
  assert(part1("./inputs/large_example_input.txt") == 10092);
  assert(part1("./inputs/input.txt") == 1563092);
  assert(part2("./inputs/large_example_input.txt") == 9021);
  assert(part2("./inputs/input.txt") == 1582688);
}
