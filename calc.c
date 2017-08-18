#include <stdio.h>

#include "lexer.c"
#include "parser.c"
#include "generator.c"

int main(int argc, char **argv){
  char input_buffer[255] = { 0 };
  int retain_output = 0;

  if(argc > 1) {
    int i;
    int offset = 0;
    for(i = 1; i < argc; i++) {
      if(argv[i][0] == '-') {
        if(argv[i][1] == 'r') {
          retain_output = 1;
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

  // printf("%d tokens found\n", tokens->length);

  // int i;
  // for(i = 0; i < tokens->length; i++) {
  //   printf("%s\n", tokens->list[i].value);
  // }

  ast_node *root_node = parser(tokens);

  // printf("Root node value: %s\n", root_node->param1->string_val);

  // ast_node *param1 = root_node->param1->param1;
  // printf("Param1 node value: %s\n", param1->string_val);

  // ast_node *param2 = root_node->param1->param2;
  // printf("Param2 node value: %s\n", param2->string_val);

  char *output = generate(root_node);
  // printf("%s\n", output);
  FILE *f = fopen("output.c", "w");
  fprintf(f, "%s\n", output);
  fclose(f);

#ifdef linux
  system("clang output.c -o output && ./output && rm output");
#endif

  if (!retain_output) {
    remove("output.c");
  }
}

/*    TRAVERSER   */

/*    TRANSFORMER   */

/*    CODE GENERATOR    */

/*    COMPILER    */
