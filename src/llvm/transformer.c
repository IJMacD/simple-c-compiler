#include <stdio.h>
#include <stdlib.h>

#include "../common/transformer.h"

ast_node *visitor_enter(ast_node *node, ast_node *parent);
ast_node *visitor_leave(ast_node *node, ast_node *parent);

int next_var_num = 1;

/***************************************
 * Below is a simple linked list
 * variable length array implementation
 ****************************************/
typedef struct node_list {
  ast_node *node;
  struct node_list* next;
} node_list;

node_list* head = NULL;
node_list* tail = NULL;
int body_node_count = 0;

void add_node (ast_node* node) {
  node_list* new_item = malloc(sizeof(node_list));
  new_item->node = node;
  new_item->next = NULL;

  if (head == NULL) {
    head = new_item;
    tail = new_item;
  } else {
    tail->next = new_item;
    tail = new_item;
  }

  body_node_count++;
}

/*    TRANSFORMER   */
ast_node *transformer(ast_node *node) {
  return traverser(node, NULL, visitor_leave);
}

ast_node *visitor_leave(ast_node *node, ast_node *parent) {

  if (node->type == NODE_OPERATOR) {

    if (parent->type != NODE_OPERATOR &&
        parent->type != NODE_PROGRAM) {
      // We only care about these two cases
      // If it's not one of these just return the node straight away to
      // avoid  unneccessary assignement

      return node;
    }

    char * var_name = calloc(32, 1);
    sprintf(var_name, "%%%d", next_var_num++);

    ast_node *name_node = make_node(NODE_NAME, 0, var_name, 0);
    free(var_name);

    ast_node *assignment_node = make_node(NODE_ASSIGNMENT, 0, NULL, 0);
    assignment_node->param1 = name_node;
    assignment_node->param2 = node;

    ast_node *statement_node = make_node(NODE_STATEMENT, 0, NULL, 1);
    add_child_node(statement_node, assignment_node);

    // Operators need to be at the root, if this is not the case we need
    // to add it to our dynamic list.
    // Then return a new name node to replace ourself.
    if (parent->type == NODE_OPERATOR) {
      add_node(statement_node);

      return make_node(NODE_NAME, 0, var_name, 0);
    }

    // If we've got to here it means we were the original root
    // operator node. In which case just replace ourself with the statement node.
    return statement_node;
  }

  if (node->type == NODE_PROGRAM) {
    // When we're leaving the program node we now have a complete
    // list of statement nodes.
    // The next step is to clone the program node with space for
    // our extra nodes and add a call statement.

    int child_count = body_node_count + 2;

    ast_node* root_node = make_node(NODE_PROGRAM, 0, NULL, child_count);

    // Traverse our linked list to add children
    if (head != NULL) {
      node_list* curr = head;
      do {
        add_child_node(root_node, curr->node);
      }
      while ((curr = curr->next) != NULL);
    }

    ast_node *param_node;

    // Add the original body node at the end
    if (node->body[0]->type == NODE_STATEMENT) {
      // Only statement nodes can be added to the root
      add_child_node(root_node, node->body[0]);

      // Get the name node, from the assignment node, from
      // the statment node, from the program node
      ast_node *name_node = node->body[0]->body[0]->param1;

      param_node = make_node(NODE_NAME, 0, name_node->string_val, 0);

    } else if (node->body[0]->type == NODE_NUMBER) {
      // There is the possibility (e.g. due to constant folding)
      // our node is just a numeric node.
      // In which case we can just use it directly as the param
      // node for printf

      param_node = node->body[0];
    }

    // Call printf
    ast_node* call_node = make_node(NODE_CALL, 0, "printf", 0);
    call_node->param1 = NULL; // this will be ignored by generator
    call_node->param2 = param_node;

    add_child_node(root_node, call_node);

    node->body[0] = NULL;
    free_node(node);

    return root_node;
  }

  return node;
}
