#include "./lib.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day 3\n");
  assert(part1("./inputs/part1_example_input.txt") == 161);
  assert(part1("./inputs/input.txt") == 181345830);
  assert(part2("./inputs/part2_example_input.txt") == 48);
  assert(part2("./inputs/input.txt") == 98729041);
}
