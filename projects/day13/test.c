#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 13\n");
  assert(part1("./inputs/example_input.txt") == 480);
  assert(part1("./inputs/input.txt") == 37686);
  assert(part2("./inputs/example_input.txt") == 875318608908);
  assert(part2("./inputs/input.txt") == 77204516023437);
}
