#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"

ast_node* operator_switcher(ast_node *, ast_node *);
ast_node* constant_folder(ast_node *, ast_node *);

void compiler(const char *input, int options) {

  ast_node *root_node;
  token_list *tokens = NULL;

  if(options & OPTION_RANDOM) {
    root_node = randomiser();
  }
  else {
    tokens = lexer(input);

    if (options & OPTION_VERBOSE) {
      fprintf(stderr, "%d tokens found\n", tokens->length);

      int i;
      for(i = 0; i < tokens->length; i++) {
        fprintf(stderr, "%s\n", tokens->list[i].value);
      }
    }

    root_node = parser(tokens);

    if(tokens != NULL) {
      free_tokens(tokens);
    }
  }


  if (options & OPTION_VERBOSE) {
    debug_node(root_node);
  }

  if (options & OPTION_CONSTANT_FOLDING) {
    root_node = traverser(root_node, NULL, constant_folder);
  }

  if(options & OPTION_GRAPH) {
    if (options & OPTION_PRINT) {
      grapher(stdout, root_node);
    }
    else {
      FILE *fd = fopen("output.dot", "w");
      grapher(fd, root_node);
      fclose(fd);

  #ifdef linux
      system("dot -Tpng -ooutput.png output.dot");
  #endif

      if (!(options & OPTION_RETAIN)) {
        remove("output.dot");
      }
    }
  }

  if(options & OPTION_EXECUTE) {
    executor(root_node);
  }

  if (options & OPTION_COMPILE) {

    if (options & OPTION_TRANSFORM) {
      root_node = traverser(root_node, operator_switcher, NULL);
    }

    // Do target specific transformation
    root_node = transformer(root_node);

    if (options & OPTION_VERBOSE) {
      debug_node(root_node);
    }

    int linker_flags = 0;

    char *program = generator(root_node, &linker_flags);

    char *output = linker(program, linker_flags);

    if(options & OPTION_PRINT) {
      fprintf(stdout, "%s\n", output);
    }
    else {
      runner(output, options & OPTION_RETAIN);
    }

    if (program == output) {
      free(program);
    } else {
      free(program);
      free(output);
    }
  }
  free_node(root_node);
}

ast_node* operator_switcher(ast_node *node, ast_node *parent) {
  if (node->type == NODE_OPERATOR) {
    node->type = NODE_CALL;

    char *string_val = node->string_val;

    if (string_val[0] == '+') {
      node->string_val = strdup("add");
    }
    else if (string_val[0] == '-') {
      node->string_val = strdup("subtract");
    }
    else if (string_val[0] == '*') {
      node->string_val = strdup("multiply");
    }
    else if (string_val[0] == '/') {
      node->string_val = strdup("divide");
    }

    free(string_val);
  }

  return node;
}

ast_node* constant_folder(ast_node *node, ast_node *parent) {

  if (node->type == NODE_OPERATOR) {
    if (node->param1 != NULL &&
        node->param1->type == NODE_NUMBER &&
        node->param2 != NULL &&
        node->param2->type == NODE_NUMBER) {

      long result;

      switch(node->string_val[0]) {
        case '+':
          result = node->param1->int_val + node->param2->int_val;
          break;
        case '-':
          result = node->param1->int_val - node->param2->int_val;
          break;
        case '*':
          result = node->param1->int_val * node->param2->int_val;
          break;
        case '/':
          if (node->param2->int_val == 0) {
            // If divide by zero pass it on.
            // It's not our problem...
            return node;
          }
          result = node->param1->int_val / node->param2->int_val;
          break;
        default:
          // Didn't recognise the operator so just return the original node
          return node;
      }

      free_node(node);

      return make_node(NODE_NUMBER, result, NULL, 0);
    }
  }

  return node;
}
