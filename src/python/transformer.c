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

    ast_node *format_node = make_node(NODE_CALL, 0, "'{}'.format", 0);
    format_node->param1 = node;

    ast_node *print_node = make_node(NODE_CALL, 0, "print", 0);
    print_node->param1 = format_node;

    ast_node *statement_node = make_node(NODE_STATEMENT, 0, NULL, 1);
    add_child_node(statement_node, print_node);

    return statement_node;
  }
  else if (node->type == NODE_OPERATOR && node->string_val[0] == '^') {
    free(node->string_val);

    node->string_val = malloc(3);
    strcpy(node->string_val, "**");

    return node;
  }
  else if (node->type == NODE_OPERATOR && node->string_val[0] == '!') {
    free(node->string_val);

    node->type = NODE_CALL;

    char const name[] = "factorial";
    node->string_val = malloc(sizeof(name) + 1);
    strcpy(node->string_val, name);

    return node;
  }

  return node;
}
