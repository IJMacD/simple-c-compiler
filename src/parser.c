#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/*    PARSER    */
ast_node* parser(token_list *tokens) {
  ast_node *root_node = make_node(NODE_PROGRAM, 0, NULL, 1);

  int index = 0;

  add_child_node(root_node, walk(&index, tokens));

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
      ast_node *node = make_node(NODE_CALL, 0, current.value, 0);

      (*index)++;

      node->param1 = walk(index, tokens);

      (*index)++;

      node->param2 = walk(index, tokens);

      return node;
    }

    if(current.type == TOKEN_NUMBER) {
      return make_node(NODE_NUMBER, atol(current.value), NULL, 0);
    }

    if(current.type == TOKEN_STRING) {
      return make_node(NODE_STRING, 0, current.value, 0);
    }

    if(current.type == TOKEN_OPERATOR) {
      ast_node *node = make_node(NODE_OPERATOR,0, current.value, 0);

      (*index)++;

      node->param1 = walk(index, tokens);

      if(current.value[0] != '!') {
        (*index)++;

        node->param2 = walk(index, tokens);
      }

      return node;
    }

    fprintf(stderr, "Parser Error: Unrecognised Token: %d.\n", current.type);
    exit(-1);
  }

  return NULL;
}

ast_node * make_node(NODE_TYPE type, long int_val, char * string_val, int child_count) {
  ast_node *new_node = malloc(sizeof(ast_node) + child_count * sizeof(ast_node *));

  new_node->type = type;
  new_node->int_val = int_val;

  if (string_val != NULL) {

    char *s = malloc(strlen(string_val) + 1);
    strcpy(s, string_val);

    new_node->string_val = s;
  } else {
    new_node->string_val = NULL;
  }

  new_node->param1 = NULL;
  new_node->param2 = NULL;

  new_node->body_length = 0;

  return new_node;
}

void add_child_node(ast_node *parent, ast_node *child) {
  parent->body[parent->body_length++] = child;
}

void free_node(ast_node *node) {
  if (node == NULL) {
    // BUG detected: Why were we given a null node?
    return;
  }

  int i;
  for(i = 0; i < node->body_length; i++) {
    if (node->body[i] != NULL) {
      free_node(node->body[i]);
    }
  }

  if(node->param1 != NULL) free_node(node->param1);
  if(node->param2 != NULL) free_node(node->param2);

  if (node->string_val != NULL) {
    free(node->string_val);
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
    fprintf(stderr, "%sNumber: %ld\n", prefix, node->int_val);
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
  else if(node->type == NODE_CAST) {
    fprintf(stderr, "%sCast: %s\n", prefix, node->string_val);
    if(node->param1 != NULL)
      debug_node_val(node->param1, depth + 1);
  }
  else if(node->type == NODE_NAME) {
    fprintf(stderr, "%sName: %s\n", prefix, node->string_val);
  }
  else if(node->type == NODE_ASSIGNMENT) {
    fprintf(stderr, "%sAssignment\n", prefix);
    if(node->param1 != NULL)
      debug_node_val(node->param1, depth + 1);
    if(node->param2 != NULL)
      debug_node_val(node->param2, depth + 1);
  }
  else {
    fprintf(stderr, "%sUnknown Node: %d\n", prefix, node->type);
    // if(node->body[0] != NULL)
    //   debug_node_val(node->body[0], depth);
  }
}
