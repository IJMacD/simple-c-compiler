#include <stdio.h>

#include "lexer.c"
#include "parser.c"
#include "generator.c"

int main(){
  token_list *tokens = lexer("add 5 subtract 4 2");

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

  system("clang output.c -o output && ./output");
}

/*    TRANSVERSER   */

/*    TRANSFORMER   */

/*    CODE GENERATOR    */

/*    COMPILER    */
