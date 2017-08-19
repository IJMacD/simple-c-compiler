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

    ast_node *print_node = malloc(sizeof(ast_node));

    print_node->type = NODE_CALL;

    print_node->string_val = malloc(7);
    memcpy(print_node->string_val, "printf", 7);

    ast_node *format_node = malloc(sizeof(ast_node));

    format_node->type = NODE_STRING;

    format_node->string_val = malloc(5);
    memcpy(format_node->string_val, "%d\\n", 5);

    print_node->param1 = format_node;
    print_node->param2 = node;

    ast_node *statement_node = malloc(sizeof(ast_node) + sizeof(ast_node *));

    statement_node->type = NODE_STATEMENT;

    statement_node->body[0] = print_node;
    statement_node->body_length++;

    parent->body[0] = statement_node;

    return print_node;
  }

  return node;
}
