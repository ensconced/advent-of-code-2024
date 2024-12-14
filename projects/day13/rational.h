#pragma once
#include <stdbool.h>

typedef struct rational {
  long num;
  long denom;
} rational;

rational simplify(rational a);
bool is_integer(rational a);
rational from_unsigned_long(unsigned long a);
long to_long(rational a);
rational multiply(rational a, rational b);
rational divide(rational a, rational b);
rational negate(rational a);
rational sum(rational a, rational b);
rational subtract(rational a, rational b);
