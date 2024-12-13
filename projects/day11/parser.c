#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>

#define MAX_LEN 16

parsed_input parse_input(char *input_path) {
  char *str = read_text_file(input_path);
  unsigned long *numbers = malloc(sizeof(unsigned long long) * MAX_LEN);
  size_t numbers_len = 0;
  take_separated_numbers(&str, " ", numbers, &numbers_len, MAX_LEN);

  unsigned long long *long_numbers =
      malloc(sizeof(unsigned long long) * numbers_len);
  for (size_t i = 0; i < numbers_len; i++) {
    long_numbers[i] = (unsigned long long)numbers[i];
  }

  return (parsed_input){.numbers = long_numbers, .len = numbers_len};
}
