#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) { return *(int *)a - *(int *)b; }

int part1(char *input_file) {
  FileLines lines = read_file_lines(input_file);
  int *list_a = malloc(lines.line_count * sizeof(int));
  int *list_b = malloc(lines.line_count * sizeof(int));

  for (size_t i = 0; i < lines.line_count; i++) {
    char *part_a = lines.lines[i];
    int a = (int)take_number(&part_a);
    maybe_take_whitespace(&part_a);
    int b = (int)take_number(&part_a);
    list_a[i] = a;
    list_b[i] = b;
  }

  qsort(list_a, lines.line_count, sizeof(int), compare);
  qsort(list_b, lines.line_count, sizeof(int), compare);

  int sum = 0;
  for (size_t i = 0; i < lines.line_count; i++) {
    sum += abs(list_a[i] - list_b[i]);
  }
  return sum;
}
