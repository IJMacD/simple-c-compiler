#include <stdio.h>

#include "lexer.c"
#include "parser.c"
#include "traverser.c"
#include "transformer.c"
#include "generator.c"
#include "linker.c"
#include "executor.c"

#define MAX_INPUT_SIZE 1024

#define OPTION_RETAIN     1
#define OPTION_PRINT      2
#define OPTION_VERBOSE    4
#define OPTION_TRANSFORM  8
#define OPTION_EXECUTE   16

void compiler(const char*, int);
ast_node* operator_switcher(ast_node *, ast_node *);
void display_help(FILE *, char *);

int main(int argc, char **argv){
  char input_buffer[MAX_INPUT_SIZE] = { 0 };
  int compiler_options = 0;

  if(argc > 1) {
    int i;
    int offset = 0;
    for(i = 1; i < argc; i++) {
      if(argv[i][0] == '-' && argv[i][1] != '\0') {
        switch(argv[i][1]) {
          case 'r':
            compiler_options |= OPTION_RETAIN;
            break;
          case 'p':
            compiler_options |= OPTION_PRINT;
            break;
          case 'v':
            compiler_options |= OPTION_VERBOSE;
            break;
          case 't':
            compiler_options |= OPTION_TRANSFORM;
            break;
          case 'x':
            compiler_options |= OPTION_EXECUTE;
            break;
          case 'h':
            display_help(stdout, argv[0]);
            exit(0);
          default:
            fprintf(stderr, "Unknown option %s\n", argv[i]);
            display_help(stderr, argv[0]);
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
    fgets(input_buffer, MAX_INPUT_SIZE, stdin);
    input_buffer[MAX_INPUT_SIZE - 1] = '\0';
  }

  if(!strlen(input_buffer)) {
    display_help(stderr, argv[0]);
    exit(-1);
  }

  compiler(input_buffer, compiler_options);
}

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

void display_help(FILE *fd, char *name) {
  fprintf(fd,
  "Usage: %s [OPTIONS] \"PROGRAM\"\n\n"
  "MancCALC Simple Tokenizer, Parser, Traverser, Transformer,\n"
  "Generator, Linker, Executor\n\n"
  "\t-r\tRetain output source (don't auto-delete)\n"
  "\t-p\tPrint generated source to stdout, don't compile or execute!\n"
  "\t-v\tVerbose output (display tokens and AST)\n"
  "\t-t\tTransform AST (to function based rather than operator based)\n"
  "\t-x\tExecute the raw AST (don't generate, link or compile)\n"
  "\t-h\tDisplay this help text\n\n"
  "Example:\n"
  "\t%s -p \"add 5 subtract 4 2\"\n", name, name);
}
