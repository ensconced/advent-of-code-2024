#include "./lib.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day1\n");
  assert(part1("inputs/example_input.txt") == 11);
  assert(part1("inputs/input.txt") == 2264607);
}
