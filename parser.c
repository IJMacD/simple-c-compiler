#include <stdio.h>
#include <stdlib.h>

#define NODE_PROGRAM 1
#define NODE_CALL 2
#define NODE_NUMBER 3
#define NODE_STRING 4
#define NODE_OPERATOR 5
#define NODE_STATEMENT 6

typedef struct ast_node_struct {
  int type;
  int int_val;
  char *string_val;
  struct ast_node_struct *param1;
  struct ast_node_struct *param2;
  int body_length;
  struct ast_node_struct *body[];
} ast_node;

ast_node* walk(int*, token_list *);
void free_node(ast_node *);
void debug_node(ast_node *);
void debug_node_val(ast_node *, int);

/*    PARSER    */
ast_node* parser(token_list *tokens) {
  ast_node *root_node = malloc(sizeof(ast_node) + sizeof(ast_node *));
  root_node->type = NODE_PROGRAM;

  int index = 0;

  root_node->body[0] = walk(&index, tokens);
  root_node->body_length = 1;

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

    if(current.type == TOKEN_OPERATOR) {
      ast_node *node = malloc(sizeof(ast_node));
      node->type = NODE_OPERATOR;
      node->string_val = current.value;

      (*index)++;

      node->param1 = walk(index, tokens);

      (*index)++;

      node->param2 = walk(index, tokens);

      return node;
    }

    printf("Parser Error: Unrecognised Token: %d.\n", current.type);
    exit(-1);
  }

  return NULL;
}

void free_node(ast_node *node) {
  if(node->type == NODE_PROGRAM) {
    int i;
    for(i = 0; i < node->body_length; i++) {
      if (node->body[i] != NULL) {
        free_node(node->body[i]);
      }
    }
  }
  else if(node->type == NODE_STATEMENT) {
    if (node->body[0] != NULL) {
      free_node(node->body[0]);
    }
  } else if(node->type == NODE_CALL) {
    free_node(node->param1);
    free_node(node->param2);
  } else if(node->type == NODE_OPERATOR) {
    free_node(node->param1);
    free_node(node->param2);
  }
  free(node);
}

void debug_node (ast_node *node) {
  debug_node_val(node, 0);
}

void debug_node_val(ast_node *node, int depth) {
  char prefix[10] = { 0 };
  memset(prefix, ' ', depth);

  if(node->type == NODE_PROGRAM) {
    printf("%sProgram Node\n", prefix);
    int i;
    for(i = 0; i < node->body_length; i++) {
      if(node->body[i] != NULL)
        debug_node_val(node->body[i], depth + 1);
    }
  }
  else if(node->type == NODE_STATEMENT) {
    printf("%sStatement\n", prefix);
    if(node->body[0] != NULL)
      debug_node_val(node->body[0], depth + 1);
  }
  else if(node->type == NODE_CALL) {
    printf("%sCall: %s\n", prefix, node->string_val);
    if(node->param1 != NULL)
      debug_node_val(node->param1, depth + 1);
    if(node->param2 != NULL)
      debug_node_val(node->param2, depth + 1);
  }
  else if (node->type == NODE_NUMBER) {
    printf("%sNumber: %d\n", prefix, node->int_val);
  }
  else if (node->type == NODE_STRING) {
    printf("%sString: \"%s\"\n", prefix, node->string_val);
  }
  else if(node->type == NODE_OPERATOR) {
    printf("%sOperator: %s\n", prefix, node->string_val);
    if(node->param1 != NULL)
      debug_node_val(node->param1, depth + 1);
    if(node->param2 != NULL)
      debug_node_val(node->param2, depth + 1);
  }
  else {
    if(node->body[0] != NULL)
      debug_node_val(node->body[0], depth);
  }
}
