#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 6\n");
  assert(part1("./inputs/example_input.txt") == 41);
  assert(part1("./inputs/input.txt") == 5534);
}
