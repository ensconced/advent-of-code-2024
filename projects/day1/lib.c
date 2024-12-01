#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) { return *(int *)a - *(int *)b; }

typedef struct sorted_lists {
  int *a;
  int *b;
  size_t len;
} sorted_lists;

sorted_lists get_sorted_lists(char *input_file) {
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

  return (sorted_lists){
      .a = list_a,
      .b = list_b,
      .len = lines.line_count,
  };
}

void destroy_sorted_lists(sorted_lists *lists) {
  free(lists->a);
  free(lists->b);
}

int part1(char *input_file) {
  sorted_lists lists = get_sorted_lists(input_file);

  int sum = 0;
  for (size_t i = 0; i < lists.len; i++) {
    sum += abs(lists.a[i] - lists.b[i]);
  }

  destroy_sorted_lists(&lists);

  return sum;
}

int part2(char *input_file) {
  sorted_lists lists = get_sorted_lists(input_file);

  int sum = 0;

  size_t a_idx = 0;
  size_t b_idx = 0;

  while (a_idx < lists.len) {
    // get length of the run in a
    size_t a_run_start = a_idx;
    int val = lists.a[a_run_start];
    while (a_idx < lists.len && lists.a[a_idx] == val) {
      a_idx++;
    }
    size_t a_run_len = a_idx - a_run_start;

    // move to start of the run in b
    while (b_idx < lists.len && lists.b[b_idx] < val) {
      b_idx++;
    }
    // get length of the run in b
    size_t b_run_start = b_idx;
    while (b_idx < lists.len && lists.b[b_idx] == val) {
      b_idx++;
    }
    size_t b_run_len = b_idx - b_run_start;

    sum += val * (int)a_run_len * (int)b_run_len;
  }

  destroy_sorted_lists(&lists);

  return sum;
}
