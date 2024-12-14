#include "./lib.h"
#include "./parser.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct matrix2x2 {
  rational a;
  rational b;
  rational c;
  rational d;
} matrix2x2;

vec mat_multiply(matrix2x2 m, vec v) {
  return (vec){
      .x = sum(multiply(m.a, v.x), multiply(m.b, v.y)),
      .y = sum(multiply(m.c, v.x), multiply(m.d, v.y)),
  };
}

rational determinant(matrix2x2 m) {
  return subtract(multiply(m.a, m.d), multiply(m.b, m.c));
}

matrix2x2 invert(matrix2x2 m) {
  rational det = determinant(m);
  if (det.num == 0) {
    fprintf(stderr, "cannot invert matrix\n");
    exit(1);
  }

  return (matrix2x2){
      .a = divide(m.d, det),
      .b = divide(negate(m.b), det),
      .c = divide(negate(m.c), det),
      .d = divide(m.a, det),
  };
}

unsigned long find_minimum_tokens(parsed_input input, bool part2) {
  unsigned long result = 0;
  for (size_t i = 0; i < input.game_len; i++) {
    game g = input.games[i];

    if (part2) {
      g.prize.x = sum(g.prize.x, from_unsigned_long(10000000000000));
      g.prize.y = sum(g.prize.y, from_unsigned_long(10000000000000));
    }

    matrix2x2 A = {
        .a = g.a.x,
        .b = g.b.x,
        .c = g.a.y,
        .d = g.b.y,
    };
    vec solution = mat_multiply(invert(A), g.prize);
    if (is_integer(solution.x) && is_integer(solution.y)) {
      long x = to_long(solution.x);
      long y = to_long(solution.y);
      if (x >= 0 && y >= 0 && (part2 || (x <= 100 && y <= 100))) {
        result += (unsigned long)x * 3 + (unsigned long)y;
      }
    }
  }
  return result;
}

unsigned long part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  return find_minimum_tokens(input, false);
}

unsigned long part2(char *input_path) {
  parsed_input input = parse_input(input_path);
  return find_minimum_tokens(input, true);
}
