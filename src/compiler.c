#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

void compiler(const char *input, int options) {

  token_list *tokens = lexer(input);

  if (options & OPTION_VERBOSE) {
    fprintf(stderr, "%d tokens found\n", tokens->length);

    int i;
    for(i = 0; i < tokens->length; i++) {
      fprintf(stderr, "%s\n", tokens->list[i].value);
    }
  }

  ast_node *root_node = parser(tokens);

  if (options & OPTION_VERBOSE) {
    debug_node(root_node);
  }

  if(options & OPTION_EXECUTE) {
    executor(root_node);
    free_tokens(tokens);
    free_node(root_node);
    return;
  }

  if(options & OPTION_GRAPH) {
    if (options & OPTION_PRINT) {
      grapher(root_node, stdout);
    }
    else {
      FILE *fd = fopen("output.dot", "w");
      grapher(root_node, fd);
      fclose(fd);

  #ifdef linux
      system("dot -Tpng -ooutput.png output.dot");
  #endif

      if (!(options & OPTION_RETAIN)) {
        remove("output.dot");
      }
    }

    free_tokens(tokens);
    free_node(root_node);
    return;
  }

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

  free_tokens(tokens);
  free_node(root_node);
  free(program);

  if(options & OPTION_PRINT) {
    fprintf(stdout, "%s\n", output);
  }
  else {
    FILE *f = fopen("output.c", "w");
    fprintf(f, "%s", output);
    fclose(f);

#ifdef linux
    system("clang output.c -o output && ./output && rm output");
#endif

    if (!(options & OPTION_RETAIN)) {
      remove("output.c");
    }
  }

  free(output);
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
