#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../generator.h"

#define MAX_OUTPUT_SIZE 1024

char* generate(ast_node *);
void generate_program(ast_node *, char*, size_t*);
char* generate_statement(ast_node *, char*, size_t*);
char* generate_call(ast_node *, char*, size_t*);
char* generate_number(ast_node *, char*, size_t*);
char* generate_string(ast_node *, char*, size_t*);
char* generate_operator(ast_node *, char*, size_t*);
char* generate_cast(ast_node *, char*, size_t*);

int *include_flags;

int next_var_num = 0;

char* generator(ast_node *node, int *flags) {
  include_flags = flags;

  return generate(node);
}

char* generate(ast_node *node) {
  if(node == NULL) {
    fprintf(stderr, "Generator Error: Missing Node.\n");
    exit(-1);
  }

  char* buffer = malloc(MAX_OUTPUT_SIZE);
  size_t offset = 0;

  if(node->type == NODE_PROGRAM) {
    generate_program(node, buffer, &offset);

    return buffer;
  }

  fprintf(stderr, "Generator Error: Invalid Node Type: `%d`. generate() must be called on PROGRAM node\n", node->type);
  exit(-1);
}

void generate_program(ast_node *node, char* output, size_t* offset) {

  if (node->body_length != 1) {
    fprintf(stderr, "Generator Error: Program did not have exactly one child node.\n");
    exit(-1);
  }

  if (node->body[0] == NULL) {
    fprintf(stderr, "Generator Error: Missing program child node.\n");
    exit(-1);
  }

  if (node->body[0]->type != NODE_OPERATOR) {
    fprintf(stderr, "Generator Error: Program child node is not an operator.\n");
    exit(-1);
  }

  char* var_name = generate_operator(node->body[0], output, offset);

  output[*offset] = '\0';
}

char* generate_statement(ast_node *node, char* output, size_t* offset) {
  fprintf(stderr, "Generator Error: statement not implemented.\n");
  exit(1);
}

char* generate_call (ast_node *node, char* output, size_t* offset) {
  fprintf(stderr, "Generator Error: call not implemented.\n");
  exit(1);
}

char* generate_number(ast_node *node, char* output, size_t* buffer) {
  char *buff = calloc(32, 1);
  sprintf(buff, "%ld", node->int_val);
  return buff;
}

char* generate_string(ast_node *node, char* output, size_t* offset) {
  fprintf(stderr, "Generator Error: string not implemented.\n");
  exit(1);
}

char* generate_operator (ast_node *node, char* output, size_t* offset) {
  if(node->param1 == NULL) {
    fprintf(stderr, "Generator Error: `%s` Missing first operand.\n", node->string_val);
    exit(-1);
  }

  char* param1;

  if (node->param1->type == NODE_OPERATOR) {
    param1 = generate_operator(node->param1, output, offset);
  } else if (node->param1->type == NODE_NUMBER) {
    param1 = generate_number(node->param1, output, offset);
  }

  if (node->string_val[0] == '!') {
    fprintf(stderr, "Generator Error: Target has no `%s` operator.\n", node->string_val);
    exit(-1);
  }

  if(node->param2 == NULL) {
    fprintf(stderr, "Generator Error: `%s` Missing second operand.\n", node->string_val);
    exit(-1);
  }

  char* param2;

  if (node->param2->type == NODE_OPERATOR) {
    param2 = generate_operator(node->param2, output, offset);
  } else if (node->param2->type == NODE_NUMBER) {
    param2 = generate_number(node->param2, output, offset);
  }

  char* var_name = calloc(32, 1);
  sprintf(var_name, "%%%d", next_var_num++);

  append(output, offset, var_name);
  append(output, offset, " = ");

  switch(node->string_val[0]) {
    case '+':
      append(output, offset, "add i64 ");
      break;
    case '-':
      append(output, offset, "sub i64 ");
      break;
    case '*':
      append(output, offset, "mul i64 ");
      break;
    case '/':
      append(output, offset, "sdiv i64 ");
      break;
    default:
      fprintf(stderr, "Generator Error: Unsupported operator `%s`.\n", node->string_val);
      exit(-1);
  }

  append(output, offset, param1);
  output[(*offset)++] = ',';
  output[(*offset)++] = ' ';
  append(output, offset, param2);
  output[(*offset)++] = '\n';

  free(param1);
  free(param2);

  return var_name;
}

char* generate_cast (ast_node *node, char* output, size_t* offset) {
  fprintf(stderr, "Generator Error: Cast not implemented.\n");
  exit(-1);
}
