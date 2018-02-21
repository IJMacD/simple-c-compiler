#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "calc.h"

int main(int argc, char **argv){
  char input_buffer[MAX_INPUT_SIZE] = { 0 };
  int compiler_options = 0;

#ifdef linux
#include <unistd.h>
  srand(time(NULL) * getpid());
#else
  srand(time(NULL));
#endif

  if(argc > 1) {
    int i;
    int offset = 0;
    for(i = 1; i < argc; i++) {
      if(argv[i][0] == '-' && argv[i][1] != '\0' && argv[i][1] != ' ') {
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
          case 'g':
            compiler_options |= OPTION_GRAPH;
            break;
          case 'c':
            compiler_options |= OPTION_COMPILE;
            break;
          case 'u':
            compiler_options |= OPTION_RANDOM;
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

  if (!(compiler_options & OPTION_GRAPH) &&
      !(compiler_options & OPTION_COMPILE)){
    compiler_options |= OPTION_EXECUTE;
  }

  if(compiler_options & OPTION_RANDOM) {
    compiler(NULL, compiler_options);
  } else if(strlen(input_buffer)) {
    compiler(input_buffer, compiler_options);
  } else {
    int input_count = 0;
    while(fgets(input_buffer, MAX_INPUT_SIZE, stdin) != NULL){
      if(input_buffer[0] == '\n') {
        if(input_count == 0) {
          display_help(stderr, argv[0]);
        }
        exit(-1);
      }
      compiler(input_buffer, compiler_options);
      input_count++;
    }
  }

}

void display_help(FILE *fd, char *name) {
  fprintf(fd,
  "Usage: %s [OPTIONS] \"PROGRAM\"\n\n"
  "SimpleCalc Simple Tokenizer, Parser, Traverser, Transformer, Generator,\n"
  "Linker, Executor. If PROGRAM is not given it will expect input on stdin.\n\n"
  "\t-r\tRetain output source (don't auto-delete)\n"
  "\t-p\tPrint generated source to stdout\n"
  "\t\t(won't compile or output image file)\n"
  "\t-v\tVerbose output (display tokens and AST)\n"
  "\t-t\tTransform AST (to function based rather than operator based)\n"
  "\t-x\tExecute the raw AST (default if -c or -g not provided)\n"
  "\t-g\tCreate a graph of the raw AST\n"
  "\t\t(default creates image, with -p outputs graph source (.dot) instead)\n"
  "\t-c\tCompiles and executes AST transformed into target source code\n"
  "\t\t(with -p outputs source instead)\n"
  "\t-u\tIgnore all input and generate a random AST\n"
  "\t-h\tDisplay this help text\n\n"
  "Example:\n"
  "\t%s \"+ 5 * 7 3\"\n"
  "\t%s \"add 5 subtract 4 2\"\n", name, name, name);
}
