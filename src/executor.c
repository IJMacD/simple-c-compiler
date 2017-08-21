#include <stdio.h>
#include <stdlib.h>

#include "executor.h"

void executor (ast_node *node) {
  execute_node(node);
}

int execute_node (ast_node *node) {
  int i, a, b;
  switch(node->type) {
    case NODE_PROGRAM:
      for (i = 0; i < node->body_length; i++) {
        printf("%d\n", execute_node(node->body[i]));
      }
      return 0;
    case NODE_OPERATOR:
      a = execute_node(node->param1);
      b = execute_node(node->param2);
      switch(node->string_val[0]) {
        case '+':
          return a + b;
        case '-':
          return a - b;
        case '*':
          return a * b;
        case '/':
          return a / b;
      }
      return 0;
    case NODE_NUMBER:
      return node->int_val;
    default:
      fprintf(stderr, "Executor Error: Can't execute node type %d\n", node->type);
      exit(-1);
  }

}
