#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 9\n");
  assert(part1("./inputs/example_input.txt") == 1928);
  assert(part1("./inputs/input.txt") == 6211348208140);
  assert(part2("./inputs/example_input.txt") == 2858);
  assert(part2("./inputs/input.txt") == 6239783302560);
}
