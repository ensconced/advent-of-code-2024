#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 12\n");
  assert(part1("./inputs/example_input.txt") == 1930);
  assert(part1("./inputs/input.txt") == 1396298);
  assert(part2("./inputs/example_input.txt") == 1206);
  assert(part2("./inputs/input.txt") == 853588);
}
