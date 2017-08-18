#include <stdio.h>

#include "lexer.c"
#include "parser.c"
#include "traverser.c"
#include "generator.c"
#include "linker.c"

ast_node* operator_switcher(ast_node *);

int main(int argc, char **argv){
  char input_buffer[255] = { 0 };
  int retain_output = 0;
  int print_output = 0;
  int verbose = 0;

  if(argc > 1) {
    int i;
    int offset = 0;
    for(i = 1; i < argc; i++) {
      if(argv[i][0] == '-' && argv[i][1] != '\0') {
        if(argv[i][1] == 'r') {
          retain_output = 1;
        }
        else if(argv[i][1] == 'p') {
          print_output = 1;
        }
        else if(argv[i][1] == 'v') {
          verbose = 1;
        }
        else {
          printf("Unknown option %s\n", argv[i]);
          exit(-1);
        }
      }
      else {
        strcpy(&input_buffer[offset], argv[i]);
        offset += strlen(argv[i]);
        input_buffer[offset] = ' ';
        offset++;
      }
    }

    input_buffer[offset] = '\0';
  }

  if(!strlen(input_buffer)) {
    strcpy(input_buffer, "add 5 subtract 4 2");
  }

  token_list *tokens = lexer(input_buffer);

  if (verbose) {
    printf("%d tokens found\n", tokens->length);

    int i;
    for(i = 0; i < tokens->length; i++) {
      printf("%s\n", tokens->list[i].value);
    }
  }

  ast_node *root_node = parser(tokens);

  if (verbose) {
    debug_node(root_node);
  }

  ast_node *new_root = traverser(root_node, operator_switcher);

  if (verbose) {
    debug_node(new_root);
  }

  int linker_flags = 0;

  char *program = generator(new_root, &linker_flags);

  char *output = linker(program, linker_flags);

  free_tokens(tokens);
  free_node(root_node);
  free(program);

  if(print_output) {
    printf("%s\n", output);
  }

  FILE *f = fopen("output.c", "w");
  fprintf(f, "%s", output);
  fclose(f);

  free(output);

#ifdef linux
  system("clang output.c -o output && ./output && rm output");
#endif

  if (!retain_output) {
    remove("output.c");
  }
}

ast_node* operator_switcher(ast_node *node) {
  if (node->type == NODE_OPERATOR) {
    node->type = NODE_CALL;

    if (node->string_val[0] == '+') {
      node->string_val = malloc(4);
      memcpy(node->string_val, "add", 4);
    }
    else if (node->string_val[0] == '-') {
      node->string_val = malloc(9);
      memcpy(node->string_val, "subtract", 9);
    }
    else if (node->string_val[0] == '*') {
      node->string_val = malloc(9);
      memcpy(node->string_val, "multiply", 9);
    }
    else if (node->string_val[0] == '/') {
      node->string_val = malloc(7);
      memcpy(node->string_val, "divide", 7);
    }
  }

  return node;
}

/*    TRANSFORMER   */
