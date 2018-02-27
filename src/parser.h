#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#define NODE_PROGRAM 1
#define NODE_CALL 2
#define NODE_NUMBER 3
#define NODE_STRING 4
#define NODE_OPERATOR 5
#define NODE_STATEMENT 6
#define NODE_CAST 7

typedef struct ast_node_struct {
  int type;
  long int_val;
  char *string_val;
  struct ast_node_struct *param1;
  struct ast_node_struct *param2;
  int body_length;
  struct ast_node_struct *body[];
} ast_node;

ast_node* parser(token_list *);
ast_node* walk(int*, token_list *);
void free_node(ast_node *);
void debug_node(ast_node *);
void debug_node_val(ast_node *, int);

#endif
