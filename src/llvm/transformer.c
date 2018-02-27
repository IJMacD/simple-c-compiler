#include <stdio.h>
#include <stdlib.h>

#include "../transformer.h"

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
  return traverser(node, visitor_enter, visitor_leave);
}

ast_node *visitor_enter(ast_node *node, ast_node *parent) {

  if (node->type == NODE_PROGRAM) {

    if (node->body_length != 1) {
      fprintf(stderr, "Transformer Error: Program must have exactly one child.\n");
      exit(-1);
    }

    if (node->body[0]->type != NODE_OPERATOR) {
      fprintf(stderr, "Transformer Error: Program's only child must be an operator.\n");
      exit(-1);
    }
  }

  return node;
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

    ast_node *name_node = malloc(sizeof(ast_node));

    name_node->type = NODE_NAME;

    char * var_name = calloc(32, 1);
    sprintf(var_name, "%%%d", next_var_num++);

    name_node->string_val = var_name;

    ast_node *assignment_node = malloc(sizeof(ast_node));

    assignment_node->type = NODE_ASSIGNMENT;

    assignment_node->param1 = name_node;
    assignment_node->param2 = node;

    ast_node *statement_node = malloc(sizeof(ast_node) + 1 * sizeof(ast_node *));

    statement_node->type = NODE_STATEMENT;

    statement_node->body[0] = assignment_node;
    statement_node->body_length = 1;

    // Operators need to be at the root,
    // if this is not the case we need
    // to add it to our dynamic list
    if (parent->type == NODE_OPERATOR) {
      add_node(statement_node);

      ast_node *name_node = malloc(sizeof(ast_node));
      name_node->type = NODE_NAME;
      name_node->string_val = var_name;

      return name_node;
    }

    // If we've got to here it means we were the original root
    // operator node. In which case just replace ourself with the statement node
    return statement_node;
  }

  if (node->type == NODE_PROGRAM) {
    // When we're leaving the program node we now have a complete
    // list of statement nodes.
    // The next step is to clone the program node with space for
    // our extra nodes and add a call statement.

    int child_count = body_node_count + 2;

    ast_node* root_node = malloc(sizeof(ast_node) + child_count * sizeof(ast_node *));
    root_node->type = NODE_PROGRAM;
    root_node->body_length = child_count;

    // Traverse our linked list to add children
    int i = 0;
    if (head != NULL) {
      node_list* curr = head;
      do {
        root_node->body[i++] = curr->node;
      }
      while ((curr = curr->next) != NULL);
    }

    // Add the original body node at the end
    root_node->body[i++] = node->body[0];

    // Call printf
    ast_node *name_node = malloc(sizeof(ast_node));
    name_node->type = NODE_NAME;
    char * var_name = calloc(32, 1);
    sprintf(var_name, "%%%d", next_var_num - 1); // Get the last var assigned
    name_node->string_val = var_name;

    ast_node* call_node = malloc(sizeof(ast_node));
    call_node->type = NODE_CALL;
    char * call_name = calloc(7, 1);
    strcpy(call_name, "printf");
    call_node->string_val = call_name;
    call_node->param1 = NULL; // this will be ignored by generator
    call_node->param2 = name_node;

    root_node->body[i++] = call_node;

    return root_node;
  }

  return node;
}
