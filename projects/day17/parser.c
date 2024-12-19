#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>
#include <string.h>

parsed_input parse_input(char *input_path) {
  FileLines filelines = read_file_lines(input_path);

  char *a_line = filelines.lines[0];
  char **a_ptr = &a_line;
  take_string("Register A: ", a_ptr);
  unsigned long a = take_number(a_ptr);

  char *b_line = filelines.lines[1];
  char **b_ptr = &b_line;
  take_string("Register B: ", b_ptr);
  unsigned long b = take_number(b_ptr);

  char *c_line = filelines.lines[2];
  char **c_ptr = &c_line;
  take_string("Register C: ", c_ptr);
  unsigned long c = take_number(c_ptr);

  char *program_line = filelines.lines[4];
  char **prog_ptr = &program_line;
  take_string("Program: ", prog_ptr);

  size_t prog_buffer_capacity = strlen(program_line);
  unsigned long *prog_buffer =
      malloc(sizeof(unsigned long) * prog_buffer_capacity);
  size_t program_len = 0;
  take_separated_numbers(prog_ptr, ",", prog_buffer, &program_len,
                         prog_buffer_capacity);

  int *program = malloc(sizeof(int) * program_len);
  for (size_t i = 0; i < program_len; i++) {
    program[i] = (int)prog_buffer[i];
  }

  return (parsed_input){
      .a = (long)a,
      .b = (long)b,
      .c = (long)c,
      .program = program,
      .program_len = program_len,
  };
}
