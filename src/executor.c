#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "executor.h"

void executor (ast_node *node) {
  execute_node(node);
}

int execute_node (ast_node *node) {
  if(node == NULL) {
    fprintf(stderr, "Generator Error: Missing Node.\n");
    exit(-1);
  }

  int i, a, b;
  switch(node->type) {
    case NODE_PROGRAM:
      for (i = 0; i < node->body_length; i++) {
        printf("%d\n", execute_node(node->body[i]));
      }
      return 0;
    case NODE_OPERATOR:
      if(node->param1 == NULL) {
        fprintf(stderr, "Executor Error: `%s` Missing first operand.\n", node->string_val);
        exit(-1);
      }
      a = execute_node(node->param1);

      if (node->string_val[0] == '!') {
        if (a <= 0) return 1;
        b = 1;
        for(i = 1; i <= a; i++) b *= i;
        return b;
      }

      if(node->param2 == NULL) {
        fprintf(stderr, "Executor Error: `%s` Missing second operand.\n", node->string_val);
        exit(-1);
      }
      b = execute_node(node->param2);

      switch(node->string_val[0]) {
        case '+':
          return a + b;
        case '-':
          return a - b;
        case '*':
          return a * b;
        case '/':
          if (b == 0) {
            fprintf(stderr, "Executor Error: Divide by zero\n");
            exit(-1);
          }
          return a / b;
        case '^':
          return pow(a, b);
      }
      return 0;
    case NODE_NUMBER:
      return node->int_val;
    default:
      fprintf(stderr, "Executor Error: Can't execute node type %d\n", node->type);
      exit(-1);
  }

}
