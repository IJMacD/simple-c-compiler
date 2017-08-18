#include <stdio.h>
#include <stdlib.h>

#define NODE_PROGRAM 1
#define NODE_CALL 2
#define NODE_NUMBER 3
#define NODE_STRING 4

typedef struct ast_node_struct {
  int type;
  int int_val;
  char *string_val;
  struct ast_node_struct *param1;
  struct ast_node_struct *param2;
} ast_node;

ast_node* walk(int*, token_list *);
void free_node(ast_node *);

/*    PARSER    */
ast_node* parser(token_list *tokens) {
  ast_node *root_node = malloc(sizeof(ast_node));
  root_node->type = NODE_PROGRAM;

  int index = 0;

  root_node->param1 = walk(&index, tokens);

  if(index < tokens->length - 1){
    printf("Parsing Error: Too many tokens.\n");
    exit(-1);
  }

  return root_node;
}

ast_node* walk(int *index, token_list *tokens) {

  while(*index < tokens->length) {
    token current = tokens->list[*index];

    if(current.type == TOKEN_NAME) {
      ast_node *node = malloc(sizeof(ast_node));
      node->type = NODE_CALL;
      node->string_val = current.value;

      (*index)++;

      node->param1 = walk(index, tokens);

      (*index)++;

      node->param2 = walk(index, tokens);

      return node;
    }

    if(current.type == TOKEN_NUMBER) {
      ast_node *node = malloc(sizeof(ast_node));
      node->type = NODE_NUMBER;
      node->int_val = atoi(current.value);

      return node;
    }

    if(current.type == TOKEN_STRING) {
      ast_node *node = malloc(sizeof(ast_node));
      node->type = NODE_STRING;
      node->string_val = current.value;

      return node;
    }

    return NULL;
  }

  return NULL;
}

void free_node(ast_node *node) {
  if(node->type == NODE_PROGRAM) {
    free_node(node->param1);
  } else if(node->type == NODE_CALL) {
    free_node(node->param1);
    free_node(node->param2);
  }
  free(node);
}
