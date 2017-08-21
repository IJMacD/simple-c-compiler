#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc.h"

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
