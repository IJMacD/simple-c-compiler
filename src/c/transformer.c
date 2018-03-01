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

    ast_node *print_node = make_node(NODE_CALL, 0, "printf", 0);

    ast_node *format_node = make_node(NODE_STRING, 0, "%ld\\n", 0);

    print_node->param1 = format_node;
    print_node->param2 = node;

    ast_node *statement_node = make_node(NODE_STATEMENT, 0, NULL, 1);
    add_child_node(statement_node, print_node);

    return statement_node;
  }
  else if (node->type == NODE_OPERATOR && node->string_val[0] == '^') {

    ast_node *cast_node = make_node(NODE_CAST, 0, "int", 0);
    cast_node->param1 = node->param1;

    ast_node *call_node = make_node(NODE_CALL, 0, "pow", 0);
    cast_node->param1 = call_node;

    node->param1 = NULL;
    free_node(node);

    return cast_node;
  }
  else if (node->type == NODE_OPERATOR && node->string_val[0] == '!') {
    ast_node * call_node = make_node(NODE_CALL, 0, "factorial", 0);
    call_node->param1 = node->param1;

    node->param1 = NULL;
    free_node(node);

    return call_node;
  }

  return node;
}
