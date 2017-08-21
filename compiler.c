#include <stdio.h>

#define OPTION_RETAIN     1
#define OPTION_PRINT      2
#define OPTION_VERBOSE    4
#define OPTION_TRANSFORM  8
#define OPTION_EXECUTE   16

void compiler(const char*, int);
ast_node* operator_switcher(ast_node *, ast_node *);

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
    execute_node(root_node);
    free_tokens(tokens);
    free_node(root_node);
    exit(0);
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
    printf("%s\n", output);
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
