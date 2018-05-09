#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/generator.h"

#define BODY_SIZE 2048

int *include_flags;

char* generator(ast_node *node, int *flags) {
  include_flags = flags;

  return generate(node);
}

char* generate(ast_node *node) {
  if(node == NULL) {
    fprintf(stderr, "Generator Error: Missing Node.\n");
    exit(-1);
  }

  if(node->type == NODE_PROGRAM) {
    return generate_program(node);
  }

  if(node->type == NODE_STATEMENT) {
    return generate_statement(node);
  }

  if(node->type == NODE_CALL) {
    return generate_call(node);
  }

  if(node->type == NODE_NUMBER) {
    return generate_number(node);
  }

  if(node->type == NODE_STRING) {
    return generate_string(node);
  }

  if(node->type == NODE_OPERATOR) {
    return generate_operator(node);
  }

  if(node->type == NODE_CAST) {
    return generate_cast(node);
  }

  if(node->type == NODE_NAME) {
    return generate_name(node);
  }

  if(node->type == NODE_ASSIGNMENT) {
    return generate_assignment(node);
  }

  fprintf(stderr, "Generator Error: Unrecognised Node Type: %d\n", node->type);
  exit(-1);
}

char* generate_program(ast_node *node) {
  static const char head[] = ".assembly Hello {}\n.assembly extern mscorlib {}\n.method static void Main()\n{\n\t.entrypoint\n";
  static const char tail[] = "\tcall void [mscorlib]System.Console::WriteLine(float64)\n\tret\n}\n";

  static const int head_len = sizeof(head) - 1;
  static const int tail_len = sizeof(tail) - 1;

  char body[BODY_SIZE];
  size_t offset = 0;
  int  i;

  for(i = 0; i < node->body_length; i++) {
    if(node->body[i] != NULL){
      char *child = generate(node->body[i]);
      int len = strlen(child);
      if(offset + len > BODY_SIZE) {
        fprintf(stderr, "Generator Error: Not enough space reserved for body.\n");
        exit(-1);
      }
      append(body, &offset, child);
      free(child);
    }
  }

  body[offset] = '\0';

  int output_len = head_len + offset + tail_len + 1;

  char *output = malloc(output_len);
  offset = 0;

  append(output, &offset, head);
  append(output, &offset, body);
  append(output, &offset, tail);

  output[offset] = '\0';

  return output;
}

char* generate_statement(ast_node *node) {
  fprintf(stderr, "Generator Error: `statement` unimplemented.\n");
  exit(-1);
}

char* generate_call (ast_node *node) {
  fprintf(stderr, "Generator Error: `call` unimplemented.\n");
  exit(-1);
}

char* generate_number(ast_node *node) {
  char *output = calloc(32, sizeof(char));
  sprintf(output, "\tldc.r8 %ld\n", node->int_val);
  return output;
}

char* generate_string(ast_node *node) {
  fprintf(stderr, "Generator Error: `string` unimplemented.\n");
  exit(-1);
}

char* generate_operator (ast_node *node) {
  if(node->param1 == NULL) {
    fprintf(stderr, "Generator Error: `%s` Missing first operand.\n", node->string_val);
    exit(-1);
  }

  if(node->param1->type != NODE_NUMBER && node->param1->type != NODE_OPERATOR) {
    fprintf(stderr, "Generator Error: First operand must be a name or an operator.\n");
    exit(-1);
  }

  char *param1 = generate(node->param1);

  if(node->param2 == NULL) {
    fprintf(stderr, "Generator Error: `%s` Missing second operand.\n", node->string_val);
    exit(-1);
  }

  if(node->param2->type != NODE_NUMBER && node->param2->type != NODE_OPERATOR) {
    fprintf(stderr, "Generator Error: Second operand must be a name or an operator.\n");
    exit(-1);
  }

  char *param2 = generate(node->param2);

  if (node->string_val[0] == '!') {
    fprintf(stderr, "Generator Error: Target has no `%s` operator.\n", node->string_val);
    exit(-1);
  }

  int param1_len = strlen(param1);
  int param2_len = strlen(param2);

  char *output = malloc(param1_len + param2_len + 12);
  size_t offset = 0;

  append(output, &offset, param1);
  append(output, &offset, param2);

  switch (node->string_val[0]) {
    case '+':
      append(output, &offset, "\tadd\n");
      break;
    case '-':
      append(output, &offset, "\tsub\n");
      break;
    case '*':
      append(output, &offset, "\tmul\n");
      break;
    case '/':
      append(output, &offset, "\tdiv\n");
      break;
    default:
      fprintf(stderr, "Generator Error: Unsupported operator `%s`\n.", node->string_val);
      exit(-1);
  }

  output[offset++] = '\0';

  free(param1);
  free(param2);

  return output;
}

char* generate_cast (ast_node *node) {
  fprintf(stderr, "Generator Error: `cast` unimplemented.\n");
  exit(-1);
}

char* generate_name (ast_node* node) {
  fprintf(stderr, "Generator Error: `name` unimplemented.\n");
  exit(-1);
}

char* generate_assignment (ast_node *node) {
  fprintf(stderr, "Generator Error: `generate` unimplemented.\n");
  exit(-1);
}
