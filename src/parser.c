#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/*    PARSER    */
ast_node* parser(token_list *tokens) {
  ast_node *root_node = malloc(sizeof(ast_node) + sizeof(ast_node *));
  root_node->type = NODE_PROGRAM;

  int index = 0;

  root_node->body[0] = walk(&index, tokens);
  root_node->body_length = 1;

  if(index < tokens->length - 1){
    fprintf(stderr, "Parsing Error: Excess tokens provided.\n");
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

      if(*index + 1 < tokens->length) {
        token next = tokens->list[*index + 1];

        if(next.type == TOKEN_OPERATOR &&
            next.value[0] == '!' ){

          ast_node *fac = malloc(sizeof(ast_node));

          fac->type = NODE_OPERATOR;
          fac->string_val = next.value;
          fac->param1 = node;
          fac->param2 = NULL;

          (*index)++;

          return fac;
        }
      }

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

    fprintf(stderr, "Parser Error: Unrecognised Token: %d.\n", current.type);
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
    if(node->param1 != NULL) free_node(node->param1);
    if(node->param2 != NULL) free_node(node->param2);
  } else if(node->type == NODE_OPERATOR) {
    if(node->param1 != NULL) free_node(node->param1);
    if(node->param2 != NULL) free_node(node->param2);
  }
  free(node);
}

void debug_node (ast_node *node) {
  debug_node_val(node, 0);
}

void debug_node_val(ast_node *node, int depth) {
  char prefix[32] = { 0 };
  memset(prefix, ' ', depth);

  if(node->type == NODE_PROGRAM) {
    fprintf(stderr, "%sProgram Node\n", prefix);
    int i;
    for(i = 0; i < node->body_length; i++) {
      if(node->body[i] != NULL)
        debug_node_val(node->body[i], depth + 1);
    }
  }
  else if(node->type == NODE_STATEMENT) {
    fprintf(stderr, "%sStatement\n", prefix);
    if(node->body[0] != NULL)
      debug_node_val(node->body[0], depth + 1);
  }
  else if(node->type == NODE_CALL) {
    fprintf(stderr, "%sCall: %s\n", prefix, node->string_val);
    if(node->param1 != NULL)
      debug_node_val(node->param1, depth + 1);
    if(node->param2 != NULL)
      debug_node_val(node->param2, depth + 1);
  }
  else if (node->type == NODE_NUMBER) {
    fprintf(stderr, "%sNumber: %d\n", prefix, node->int_val);
  }
  else if (node->type == NODE_STRING) {
    fprintf(stderr, "%sString: \"%s\"\n", prefix, node->string_val);
  }
  else if(node->type == NODE_OPERATOR) {
    fprintf(stderr, "%sOperator: %s\n", prefix, node->string_val);
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
