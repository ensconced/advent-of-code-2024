#include "./parser.h"
#include "../utils/file_utils/lib.h"
#include "../utils/linked_list/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>
#include <string.h>

LinkedList *parse_input(char *input_path) {
  char *line = read_text_file(input_path);
  size_t len = strlen(line);

  LinkedList *list = malloc(sizeof(LinkedList));
  *list = (LinkedList){};

  for (size_t i = 0; i < len; i++) {
    block *b = malloc(sizeof(block));
    size_t size = (size_t)(line[i] - '0');

    if (i % 2 == 0) {
      *b = (block){
          .id = (int)i / 2,
          .size = size,
          .moved = false,

      };
    } else {
      *b = (block){
          .id = -1,
          .size = size,
      };
    }
    LinkedListNode *node = malloc(sizeof(LinkedListNode));
    *node = (LinkedListNode){.data = b};
    append(list, node);
  }

  return list;
}
