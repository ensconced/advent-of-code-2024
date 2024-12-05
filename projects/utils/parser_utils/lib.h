#pragma once
#include <stdbool.h>
#include <stddef.h>

void take_string(char *word, char **str_pointer);
bool maybe_take_string(char *word, char **str_pointer);
bool maybe_take_from_charset(char **str_pointer, char *charset);
void take_numeric_string(char **str_pointer, char *result_buffer,
                         size_t result_buffer_capacity);
void take_alphanumeric_string(char **str_pointer, char *result_buffer,
                              size_t result_buffer_capacity);
unsigned long take_number(char **str_pointer);
bool maybe_take_numeric_string(char *const *str_pointer, char *result_buffer,
                               size_t result_buffer_capacity);
bool maybe_take_number(char **str_pointer, unsigned long *result);
void take_separated_numbers(char **str_pointer, char *separators,
                            unsigned long *result_buffer,
                            size_t *result_buffer_len,
                            size_t result_buffer_capacity);
