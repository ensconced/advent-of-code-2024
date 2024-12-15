#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  assert(part1("./inputs/small_example_input.txt") == 2028);
  assert(part1("./inputs/large_example_input.txt") == 10092);
  assert(part1("./inputs/input.txt") == 1563092);
  // part2("./inputs/small_example_input.txt");
}
