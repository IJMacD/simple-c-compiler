#include <stdio.h>

#include "traverser.h"

/*    TRAVERSER   */
ast_node *traverser(ast_node *node, ast_node *(*visitor) (ast_node *, ast_node *)) {
  traverse_node(&node, NULL, visitor);
  return node;
}

void traverse_node(ast_node **node, ast_node *parent, ast_node *(*visitor) (ast_node *, ast_node *)) {

  ast_node *new_node = visitor(*node, parent);

  if(new_node->type == NODE_PROGRAM) {
    int i;
    for (i = 0; i < new_node->body_length; i++) {
      if(new_node->body[i] != NULL)
        traverse_node(&new_node->body[i], new_node, visitor);
    }
  }
  else if(new_node->type == NODE_STATEMENT) {
    if(new_node->body[0] != NULL)
      traverse_node(&new_node->body[0], new_node, visitor);
  }
  else if(new_node->type == NODE_CALL) {
    if(new_node->param1 != NULL)
      traverse_node(&new_node->param1, new_node, visitor);
    if(new_node->param2 != NULL)
      traverse_node(&new_node->param2, new_node, visitor);
  }
  else if (new_node->type == NODE_NUMBER) {
  }
  else if (new_node->type == NODE_STRING) {
  }
  else if(new_node->type == NODE_OPERATOR) {
    if(new_node->param1 != NULL)
      traverse_node(&new_node->param1, new_node, visitor);
    if(new_node->param2 != NULL)
      traverse_node(&new_node->param2, new_node, visitor);
  }

  *node = new_node;
}
