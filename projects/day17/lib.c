#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_OUTPUT_LEN 32

typedef struct computer {
  long reg_a;
  long reg_b;
  long reg_c;
  size_t instruction_ptr;
  int *program;
  size_t program_len;
  char output;
  bool output_flag;
} computer;

typedef enum opcode {
  adv = 0,
  bxl = 1,
  bst = 2,
  jnz = 3,
  bxc = 4,
  out = 5,
  bdv = 6,
  cdv = 7,
} opcode;

long combo_operand(computer *comp, int operand) {
  switch (operand) {
  case 4:
    return comp->reg_a;
  case 5:
    return comp->reg_b;
  case 6:
    return comp->reg_c;
  case 7: {
    fprintf(stderr, "invalid operand\n");
    exit(1);
  }
  default: {
    return operand;
  }
  }
}

void clock(computer *comp) {
  int opcode = comp->program[comp->instruction_ptr];
  int operand = comp->program[comp->instruction_ptr + 1];
  comp->output_flag = false;

  switch (opcode) {
  case adv: {
    comp->reg_a /= (1 << combo_operand(comp, operand));
    break;
  }
  case bxl: {
    // comp->reg_b ^= operand;
    break;
  }
  case bst: {
    // comp->reg_b = combo_operand(comp, operand) % 8;
    break;
  }
  case jnz: {
    if (comp->reg_a != 0) {
      comp->instruction_ptr = (size_t)operand;
    }
    break;
  }
  case bxc: {
    // comp->reg_b ^= comp->reg_c;
    break;
  }
  case out: {
    comp->output = (char)(combo_operand(comp, operand) % 8);
    comp->output_flag = true;
    break;
  }
  case bdv: {
    // comp->reg_b = comp->reg_a / (1 << combo_operand(comp, operand));
    break;
  }
  case cdv: {
    // comp->reg_c = comp->reg_a / (1 << combo_operand(comp, operand));
    break;
  }
  }

  if (opcode != jnz) {
    comp->instruction_ptr += 2;
  }
}

void run(computer *comp, char *output, size_t *output_len) {
  while (comp->instruction_ptr < comp->program_len - 1) {
    computer prev_state = *comp;
    clock(comp);
    // In theory we could also enter a longer loop...but we never seem to.
    if (prev_state.instruction_ptr == comp->instruction_ptr &&
        prev_state.reg_a == comp->reg_a && prev_state.reg_b == comp->reg_b &&
        prev_state.reg_c == comp->reg_c) {
      break;
    }
    if (comp->output_flag) {
      if (*output_len == MAX_OUTPUT_LEN - 1) {
        fprintf(stderr, "output buffer is full\n");
        exit(1);
      }
      output[(*output_len)++] = comp->output;
    }
  }
}

bool is_quine(computer *comp) {
  size_t output_len = 0;
  while (comp->instruction_ptr < comp->program_len - 1) {
    computer prev_state = *comp;
    clock(comp);
    if (comp->output_flag) {
      if (comp->program[output_len++] != (int)comp->output) {
        return false;
      } else if (output_len == comp->program_len) {
        return true;
      }
    }
    // In theory we could also enter a longer loop...but we never seem to.
    if (prev_state.instruction_ptr == comp->instruction_ptr &&
        prev_state.reg_a == comp->reg_a && prev_state.reg_b == comp->reg_b &&
        prev_state.reg_c == comp->reg_c) {
      return false;
    }
  }
  return false;
}

char *part1(char *input_path) {
  parsed_input input = parse_input(input_path);
  computer comp = {
      .reg_a = input.a,
      .reg_b = input.b,
      .reg_c = input.c,
      .program = input.program,
      .program_len = input.program_len,
      .instruction_ptr = 0,
      .output = 0,
      .output_flag = false,
  };
  char *output = malloc(sizeof(char) * MAX_OUTPUT_LEN + 1);
  size_t output_len = 0;
  run(&comp, output, &output_len);

  char *comma_separated = malloc(sizeof(char) * output_len * 2);
  for (size_t i = 0; i < output_len; i++) {
    comma_separated[i * 2] = output[i] + 48;
    comma_separated[i * 2 + 1] = ',';
  }
  comma_separated[output_len == 0 ? 0 : output_len * 2 - 1] = '\0';
  return comma_separated;
}

void decompile(int *program, size_t program_len) {
  for (size_t i = 0; i < program_len - 1; i += 2) {
    int opcode = program[i];
    int operand = program[i + 1];

    char *op;
    switch (opcode) {
    case adv: {
      op = "adv";
      break;
    }
    case bxl: {
      op = "bxl";
      break;
    }
    case bst: {
      op = "bst";
      break;
    }
    case jnz: {
      op = "jnz";
      break;
    }
    case bxc: {
      op = "bxc";
      break;
    }
    case out: {
      op = "out";
      break;
    }
    case bdv: {
      op = "bdv";
      break;
    }
    case cdv: {
      op = "cdv";
      break;
    }
    }

    char arg;
    switch (operand) {
    case 4:
      arg = 'a';
      break;
    case 5:
      arg = 'b';
      break;
    case 6:
      arg = 'c';
      break;
    case 7: {
      fprintf(stderr, "invalid operand\n");
      exit(1);
    }
    default: {
      arg = (char)operand + 48;
      break;
    }
    }

    printf("%2lu | %s %c\n", i, op, arg);
  }
}

long part2(char *input_path) {
  // long output[16] = {2, 4, 1, 3, 7, 5, 4, 1, 1, 3, 0, 3, 5, 5, 3, 0};
  // long i = 0;
  // while (true) {
  //   bool failed = false;
  //   for (size_t j = 0; j < 16; j++) {
  //     if ((i / 2 << (3 * j)) % 8 != output[j]) {
  //       failed = true;
  //       break;
  //     }
  //   }
  //   if (failed) {
  //     i++;
  //   } else {
  //     return i;
  //   }
  // }

  parsed_input input = parse_input(input_path);
  decompile(input.program, input.program_len);
  // return 0;

  long i = 03553031145731420;

  computer comp = {
      .reg_a = i,
      .reg_b = input.b,
      .reg_c = input.c,
      .program = input.program,
      .program_len = input.program_len,
      .instruction_ptr = 0,
      .output = 0,
      .output_flag = false,
  };

  if (is_quine(&comp)) {
    printf("success\n");
    return i;
  } else {
    return 0;
  }
}
