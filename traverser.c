#include <stdio.h>

ast_node *traverser(ast_node *, ast_node *(*visitor) (ast_node *));

/*    TRAVERSER   */
ast_node *traverser(ast_node *node, ast_node *(*visitor) (ast_node *)) {

  ast_node *new_node = visitor(node);

  if(new_node->type == NODE_PROGRAM) {
    if(new_node->param1 != NULL)
      traverser(new_node->param1, visitor);
  }
  else if(new_node->type == NODE_CALL) {
    if(new_node->param1 != NULL)
      traverser(new_node->param1, visitor);
    if(new_node->param2 != NULL)
      traverser(new_node->param2, visitor);
  }
  else if (new_node->type == NODE_NUMBER) {
  }
  else if (new_node->type == NODE_STRING) {
  }
  else if(new_node->type == NODE_OPERATOR) {
    if(new_node->param1 != NULL)
      traverser(new_node->param1, visitor);
    if(new_node->param2 != NULL)
      traverser(new_node->param2, visitor);
  }

  return new_node;
}
