#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 5\n");
  assert(part1("./inputs/example_input.txt") == 143);
  assert(part1("./inputs/input.txt") == 6242);
}
