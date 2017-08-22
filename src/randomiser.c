#include "randomiser.h"

int node_count = 0;

ast_node *randomiser() {
  ast_node *node = malloc(sizeof(ast_node) + sizeof(ast_node *));
  node->type = NODE_PROGRAM;

  node->body[0] = random_operator();
  node->body_length++;

  return node;
}

ast_node *random_operator() {
  ast_node *out = malloc(sizeof(ast_node));
  out->type = NODE_OPERATOR;

  node_count++;

  switch(rand() % 4) {
    case 0:
      out->string_val = "+";
      break;
    case 1:
      out->string_val = "-";
      break;
    case 2:
      out->string_val = "*";
      break;
    case 3:
      out->string_val = "/";
      break;
  }

  out->param1 = (rand() %  2 || node_count >= RANDOMISER_NODE_COUNT) ? random_number() : random_operator();
  out->param2 = (rand() %  2 || node_count >= RANDOMISER_NODE_COUNT) ? random_number() : random_operator();

  return out;
}

ast_node *random_number() {
  ast_node *out = malloc(sizeof(ast_node));
  out->type = NODE_NUMBER;

  node_count++;

  out->int_val = RANDOMISER_NUM_MIN + (rand() % (RANDOMISER_NUM_MAX - RANDOMISER_NUM_MIN));

  return out;
}
