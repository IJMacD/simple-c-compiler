#include <stdio.h>

ast_node *transformer(ast_node *);
ast_node *visitor(ast_node *, ast_node *);

/*    TRANSFORMER   */
ast_node *transformer(ast_node *node) {
  return traverser(node, visitor);
}

ast_node *visitor(ast_node *node, ast_node *parent) {
  // The top level call needs to be wrapped
  if (parent != NULL && parent->type == NODE_PROGRAM &&
      (node->type == NODE_CALL || node->type == NODE_OPERATOR)) {
    ast_node *new_node = malloc(sizeof(ast_node));

    new_node->type = NODE_CALL;

    new_node->string_val = malloc(7);
    memcpy(new_node->string_val, "printf", 7);

    ast_node *format_node = malloc(sizeof(ast_node));

    format_node->type = NODE_STRING;

    format_node->string_val = malloc(5);
    memcpy(format_node->string_val, "%d\\n", 5);

    new_node->param1 = format_node;
    new_node->param2 = node;

    parent->param1 = new_node;

    return new_node;
  }

  return node;
}
