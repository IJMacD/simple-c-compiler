#include <stdio.h>

#include "traverser.h"

/*    TRAVERSER   */
ast_node *traverser(ast_node *node, ast_node *(*visitor) (ast_node *, ast_node *)) {
  traverse_node(&node, NULL, visitor);
  return node;
}

void traverse_node(ast_node **node, ast_node *parent, ast_node *(*visitor) (ast_node *, ast_node *)) {

  *node = visitor(*node, parent);

  if ((*node)->type != NODE_NUMBER &&
    (*node)->type != NODE_STRING
  ) {
    // Conditional jump or move depends on unitialised value(s)
    // param1 and param2 might not have been initialised to NULL
    // they almost certainly haven't for number and string constants
    if((*node)->param1 != NULL)
      traverse_node(&(*node)->param1, *node, visitor);
    if((*node)->param2 != NULL)
      traverse_node(&(*node)->param2, *node, visitor);
  }


  if ((*node)->type != NODE_PROGRAM ||
    (*node)->type != NODE_STATEMENT
  ) {
    // Conditional jump or move depends on unitialised value(s)
    // body_length might not have been initialised to 0
    int i;
    for (i = 0; i < (*node)->body_length; i++) {
      if((*node)->body[i] != NULL)
        traverse_node(&(*node)->body[i], *node, visitor);
    }
  }
}
