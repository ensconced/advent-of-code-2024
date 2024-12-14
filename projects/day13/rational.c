#include "./rational.h"
#include <stdlib.h>

long gcd(long a, long b) {
  if (a < 0 || b < 0)
    return gcd(labs(a), labs(b));

  while (true) {
    if (a == 0)
      return b;
    if (b == 0)
      return a;
    if (a > b) {
      long times = a / b;
      a -= b * times;
    } else {
      long times = b / a;
      b -= a * times;
    }
  }
}

rational simplify(rational a) {
  long greatest_common_div = gcd(a.num, a.denom);
  return (rational){
      .num = a.num / greatest_common_div,
      .denom = a.denom / greatest_common_div,
  };
}

bool is_integer(rational a) {
  rational simplified = simplify(a);
  return simplified.denom == 1 || simplified.denom == -1;
}

rational from_unsigned_long(unsigned long a) {
  return (rational){.num = (long)a, .denom = 1};
}

long to_long(rational a) { return a.num / a.denom; }

rational multiply(rational a, rational b) {
  return (rational){
      .num = a.num * b.num,
      .denom = a.denom * b.denom,
  };
}

rational divide(rational a, rational b) {
  return multiply(a, (rational){.num = b.denom, .denom = b.num});
}

rational negate(rational a) {
  return (rational){
      .num = -a.num,
      .denom = a.denom,
  };
}

rational sum(rational a, rational b) {
  return (rational){
      .num = a.num * b.denom + b.num * a.denom,
      .denom = a.denom * b.denom,
  };
}

rational subtract(rational a, rational b) { return sum(a, negate(b)); }
