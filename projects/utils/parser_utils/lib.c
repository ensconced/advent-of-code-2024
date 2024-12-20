#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_UNSIGNED_LONG_LEN 10

bool maybe_take_from_charset(char **str_pointer, char *charset) {
  size_t taken = 0;
  while (1) {
    char next_char = (*str_pointer)[taken];
    char *match = strchr(charset, next_char);
    if (match != NULL && *match != '\0') {
      taken++;
    } else {
      break;
    }
  }
  *str_pointer += taken;
  return !!taken;
}

void take_string(char *word, char **str_pointer) {
  size_t word_len = strlen(word);
  if (strncmp(*str_pointer, word, word_len)) {
    printf("expected word \"%s\" to start with \"%s\"\n", *str_pointer, word);
    exit(EXIT_FAILURE);
  }
  *str_pointer += word_len;
}

bool maybe_take_string(char *word, char **str_pointer) {
  size_t word_len = strlen(word);
  if (strncmp(*str_pointer, word, word_len) == 0) {
    *str_pointer += word_len;
    return true;
  }
  return false;
}

void take_string_while_chars_match(char **str_pointer, char *result_buffer,
                                   size_t result_buffer_capacity,
                                   int (*test_char)(int)) {
  size_t result_len = 0;
  size_t i;
  for (i = 0;; ++i) {
    char possible_digit = (*str_pointer)[i];
    if (test_char(possible_digit)) {
      if (i >= result_buffer_capacity - 1) {
        printf("ran out of space in result buffer\n");
        exit(EXIT_FAILURE);
      }
      result_buffer[i] = possible_digit;
      result_len++;
    } else {
      break;
    }
  }
  if (result_len == 0) {
    printf("failed to take natural number\n");
    exit(EXIT_FAILURE);
  }

  result_buffer[i] = '\0';
  (*str_pointer) += result_len;
}

void take_numeric_string(char **str_pointer, char *result_buffer,
                         size_t result_buffer_capacity) {
  take_string_while_chars_match(str_pointer, result_buffer,
                                result_buffer_capacity, isdigit);
}

void take_alphanumeric_string(char **str_pointer, char *result_buffer,
                              size_t result_buffer_capacity) {
  take_string_while_chars_match(str_pointer, result_buffer,
                                result_buffer_capacity, isalnum);
}

bool maybe_take_numeric_string(char **str_pointer, char *result_buffer,
                               size_t result_buffer_capacity) {
  char first_digit = (*str_pointer)[0];
  if (isdigit(first_digit)) {
    take_numeric_string(str_pointer, result_buffer, result_buffer_capacity);
    return true;
  } else {
    result_buffer[0] = '\0';
    return false;
  }
}

unsigned long take_number(char **str_pointer) {
  static const size_t buffer_capacity = 32;
  char buffer[buffer_capacity];
  take_numeric_string(str_pointer, buffer, buffer_capacity);
  return strtoul(buffer, 0, 10);
}

long take_signed_number(char **str_pointer) {
  bool neg = maybe_take_string("-", str_pointer);
  unsigned long num = take_number(str_pointer);
  return neg ? -(long)num : (long)num;
}

bool maybe_take_number(char **str_pointer, unsigned long *result) {
  static const size_t buffer_capacity = 32;
  char buffer[buffer_capacity];
  if (maybe_take_numeric_string(str_pointer, buffer, buffer_capacity)) {
    *result = strtoul(buffer, 0, 10);
    return true;
  } else {
    return false;
  }
}

void take_separated_numeric_strings(char **str_pointer, char *separators,
                                    char **result_buffer,
                                    size_t *result_buffer_len,
                                    size_t result_buffer_capacity) {

  *result_buffer_len = 0;
  while (1) {
    char *number_buffer = malloc(MAX_UNSIGNED_LONG_LEN);
    if (number_buffer == 0) {
      printf("failed to allocate number buffer\n");
      exit(EXIT_FAILURE);
    }
    maybe_take_numeric_string(str_pointer, number_buffer,
                              MAX_UNSIGNED_LONG_LEN);
    maybe_take_from_charset(str_pointer, separators);
    if (strlen(number_buffer)) {
      if ((*result_buffer_len) > result_buffer_capacity - 1) {
        printf("ran out of space in result buffer\n");
        exit(EXIT_FAILURE);
      } else {
        result_buffer[(*result_buffer_len)++] = number_buffer;
      }
    } else {
      free(number_buffer);
      return;
    }
  }
}

void take_separated_numbers(char **str_pointer, char *separators,
                            unsigned long *result_buffer,
                            size_t *result_buffer_len,
                            size_t result_buffer_capacity) {
  char **string_array_buffer = malloc(result_buffer_capacity * sizeof(char *));
  if (string_array_buffer == 0) {
    printf("failed to allocate string array buffer\n");
    exit(EXIT_FAILURE);
  }
  take_separated_numeric_strings(str_pointer, separators, string_array_buffer,
                                 result_buffer_len, result_buffer_capacity);

  for (size_t i = 0; i < *result_buffer_len; ++i) {
    result_buffer[i] = strtoul(string_array_buffer[i], 0, 10);
  }
  free(string_array_buffer);
}
