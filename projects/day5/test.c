#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  assert(part1("./inputs/example_input.txt") == 143);
  printf("part 1: %d\n", part1("./inputs/input.txt"));
}
