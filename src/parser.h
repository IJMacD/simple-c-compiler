#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#define NODE_PROGRAM 1
#define NODE_CALL 2       // <- expression
#define NODE_NUMBER 3     // <- expression
#define NODE_STRING 4     // <- expression
#define NODE_OPERATOR 5   // <- expression
#define NODE_STATEMENT 6
#define NODE_CAST 7       // <- expression
#define NODE_NAME 8       // <- expression
#define NODE_ASSIGNMENT 9 // NAME = EXPRESSION

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
