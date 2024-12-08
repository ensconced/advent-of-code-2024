#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 8\n");
  assert(part1("./inputs/example_input.txt") == 14);
  assert(part1("./inputs/input.txt") == 329);
}
