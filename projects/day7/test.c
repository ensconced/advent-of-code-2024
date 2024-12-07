#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 7\n");
  assert(part1("./inputs/example_input.txt") == 3749);
  assert(part1("./inputs/input.txt") == 12940396350192);
  assert(part2("./inputs/example_input.txt") == 11387);
  assert(part2("./inputs/input.txt") == 106016735664498);
}
