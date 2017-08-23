#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

void compiler(const char *input, int options) {

  ast_node *root_node;
  token_list *tokens;

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
  }

  if (options & OPTION_VERBOSE) {
    debug_node(root_node);
  }

  if(options & OPTION_EXECUTE) {
    executor(root_node);
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

  if (options & OPTION_COMPILE) {

    root_node = transformer(root_node);

    if (options & OPTION_TRANSFORM) {
      root_node = traverser(root_node, operator_switcher);
    }

    if (options & OPTION_VERBOSE) {
      debug_node(root_node);
    }

    int linker_flags = 0;

    char *program = generator(root_node, &linker_flags);

    char *output = linker(program, linker_flags);

    free(program);

    if(options & OPTION_PRINT) {
      fprintf(stdout, "%s\n", output);
    }
    else {
      runner(output, options & OPTION_RETAIN);
    }

    free(output);
  }

  if(tokens != NULL) {
    free_tokens(tokens);
  }
  free_node(root_node);
}

ast_node* operator_switcher(ast_node *node, ast_node *parent) {
  if (node->type == NODE_OPERATOR) {
    node->type = NODE_CALL;

    if (node->string_val[0] == '+') {
      node->string_val = "add";
    }
    else if (node->string_val[0] == '-') {
      node->string_val = "subtract";
    }
    else if (node->string_val[0] == '*') {
      node->string_val = "multiply";
    }
    else if (node->string_val[0] == '/') {
      node->string_val = "divide";
    }
  }

  // if(node->type == NODE_PROGRAM) {
  //   realloc(node, sizeof(ast_node) + 2 * sizeof(ast_node *));
  //   node->body_length++;
  //   node->body[1] = node->body[0];
  // }

  return node;
}
