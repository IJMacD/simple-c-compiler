#include "randomiser.h"

int node_count = 0;

ast_node *randomiser() {
  ast_node *node = make_node(NODE_PROGRAM, 0, NULL, 1);

  add_child_node(node, random_operator());

  return node;
}

ast_node *random_operator() {
  node_count++;

  char * string_val;

  switch(rand() % 4) {
    case 0:
      string_val = "+";
      break;
    case 1:
      string_val = "-";
      break;
    case 2:
      string_val = "*";
      break;
    case 3:
      string_val = "/";
      break;
  }

  ast_node *out = make_node(NODE_OPERATOR, 0, string_val, 0);

  out->param1 = (rand() %  2 || node_count >= RANDOMISER_NODE_COUNT) ? random_number() : random_operator();
  out->param2 = (rand() %  2 || node_count >= RANDOMISER_NODE_COUNT) ? random_number() : random_operator();

  return out;
}

ast_node *random_number() {
  node_count++;

  return make_node(NODE_NUMBER, RANDOMISER_NUM_MIN + (rand() % (RANDOMISER_NUM_MAX - RANDOMISER_NUM_MIN)), NULL, 0);
}
