#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  // assert(strcmp(part1("./inputs/example_input.txt"), "4,6,3,5,6,3,5,2,1,0")
  // ==
  //        0);
  // assert(strcmp(part1("./inputs/input.txt"), "1,5,3,0,2,5,2,5,3") == 0);
  // assert(part2("./inputs/part2_example_input.txt") == 117440);
  // part2("./inputs/part2_example_input.txt");
  printf("%lu\n", part2("./inputs/input.txt"));
}
