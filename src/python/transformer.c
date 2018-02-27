#include <stdio.h>
#include <stdlib.h>

#include "../transformer.h"

/*    TRANSFORMER   */
ast_node *transformer(ast_node *node) {
  return traverser(node, visitor, NULL);
}

ast_node *visitor(ast_node *node, ast_node *parent) {
  // The top level call needs to be wrapped
  if (parent != NULL && parent->type == NODE_PROGRAM &&
      node->type != NODE_STATEMENT) {
    ast_node *format_node = malloc(sizeof(ast_node));

    format_node->type = NODE_CALL;

    format_node->string_val = "'{}'.format";

    format_node->param1 = node;

    ast_node *print_node = malloc(sizeof(ast_node));

    print_node->type = NODE_CALL;

    print_node->string_val = "print";

    print_node->param1 = format_node;

    ast_node *statement_node = malloc(sizeof(ast_node) + sizeof(ast_node *));

    statement_node->type = NODE_STATEMENT;

    statement_node->body[0] = print_node;
    statement_node->body_length = 1;

    return statement_node;
  }
  else if (node->type == NODE_OPERATOR && node->string_val[0] == '^') {
    node->string_val = malloc(3);
    strcpy(node->string_val, "**");
    return node;
  }
  else if (node->type == NODE_OPERATOR && node->string_val[0] == '!') {
    node->type = NODE_CALL;
    char const name[] = "factorial";
    node->string_val = malloc(sizeof(name));
    strcpy(node->string_val, name);
    return node;
  }

  return node;
}
