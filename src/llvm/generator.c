#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/generator.h"

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
  static const char head[] = "@.str = private unnamed_addr constant [5 x i8] c\"%lu\\0A\\00\", align 1\n\ndefine i32 @main() {\n";
  static const char tail[] = "\tret i32 0\n}\n\ndeclare i32 @printf(i8*, ...)\n";

  static const int head_len = sizeof(head) - 1;
  static const int tail_len = sizeof(tail) - 1;

  char body[1024];
  size_t offset = 0;
  int  i;

  for(i = 0; i < node->body_length; i++) {
    if(node->body[i] != NULL){
      char *child = generate(node->body[i]);
      int len = strlen(child);
      if(offset + len > 1024) {
        fprintf(stderr, "Generator Error: Not enough space reserved for body.\n");
        exit(-1);
      }
      body[offset++] = '\t';
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

  char *body;

  if(node->body[0] != NULL){
    body = generate(node->body[0]);
  }

  int output_len = strlen(body) + 3;

  char *output = malloc(output_len);
  size_t offset = 0;

  append(output, &offset, body);
  output[offset++] = '\n';
  output[offset++] = '\0';

  free(body);

  return output;
}

char* generate_call (ast_node *node) {
  if (strcmp(node->string_val, "printf") == 0) {

    const char * fmt = "call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i32 0, i32 0), i64 %s)\n";
    char * param;

    if (node->param2->type == NODE_NAME) {
      param = node->param2->string_val;
    } else if (node->param2->type == NODE_NUMBER) {
      param = malloc(32);
      sprintf(param, "%lu", node->param2->int_val);
    } else {
      fprintf(stderr, "Generator Error: call can only operate on named vars or numbers.\n");
      exit(-1);
    }

    char *output = malloc(strlen(fmt) - 2 + strlen(param) + 1);
    sprintf(output, fmt, param);

    return output;
  }

  fprintf(stderr, "Generator Error: call unimplemented.\n");
  exit(-1);
}

char* generate_number(ast_node *node) {
  char *output = calloc(32, sizeof(char));
  sprintf(output, "%ld", node->int_val);
  return output;
}

char* generate_string(ast_node *node) {
  fprintf(stderr, "Generator Error: string unimplemented.\n");
  exit(-1);
}

char* generate_operator (ast_node *node) {
  if(node->param1 == NULL) {
    fprintf(stderr, "Generator Error: `%s` Missing first operand.\n", node->string_val);
    exit(-1);
  }

  if(node->param1->type != NODE_NUMBER && node->param1->type != NODE_NAME) {
    fprintf(stderr, "Generator Error: First operand must be a name or a number.\n");
    exit(-1);
  }

  char *param1 = generate(node->param1);

  if(node->param2 == NULL) {
    fprintf(stderr, "Generator Error: `%s` Missing second operand.\n", node->string_val);
    exit(-1);
  }

  if(node->param2->type != NODE_NUMBER && node->param2->type != NODE_NAME) {
    fprintf(stderr, "Generator Error: Second operand must be a name or a number.\n");
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

  switch (node->string_val[0]) {
    case '+':
      append(output, &offset, "add i64 ");
      break;
    case '-':
      append(output, &offset, "sub i64 ");
      break;
    case '*':
      append(output, &offset, "mul i64 ");
      break;
    case '/':
      append(output, &offset, "sdiv i64 ");
      break;
    default:
      fprintf(stderr, "Generator Error: Unsupported operator `%s`\n.", node->string_val);
      exit(-1);
  }

  append(output, &offset, param1);
  output[offset++] = ',';
  output[offset++] = ' ';
  append(output, &offset, param2);
  output[offset++] = '\0';

  free(param1);
  free(param2);

  return output;
}

char* generate_cast (ast_node *node) {
  if(node->param1 == NULL) {
    fprintf(stderr, "Generator Error: Cast to %s missing operand.\n", node->string_val);
    exit(-1);
  }
  char *param1 = generate(node->param1);

  int type_len = strlen(node->string_val);

  int param1_len = strlen(param1);

  char *output = malloc(type_len + param1_len + 3);
  size_t offset = 0;

  output[offset++] = '(';
  append(output, &offset, node->string_val);
  output[offset++] = ')';
  append(output, &offset, param1);
  output[offset++] = '\0';

  free(param1);

  return output;
}

char* generate_name (ast_node* node) {
  char *output = calloc(strlen(node->string_val) + 1, 1);
  strcpy(output, node->string_val);
  return output;
}

char* generate_assignment (ast_node *node) {
  if(node->param1 == NULL) {
    fprintf(stderr, "Generator Error: `%s` Missing assignment target.\n", node->string_val);
    exit(-1);
  }

  if(node->param1->type != NODE_NAME) {
    fprintf(stderr, "Generator Error: Invalid assignment target.\n");
    exit(-1);
  }

  char *param1 = generate(node->param1);

  if(node->param2 == NULL) {
    fprintf(stderr, "Generator Error: `%s` Missing assignment expression.\n", node->string_val);
    exit(-1);
  }

  // These are all the valid expression types
  if(node->param2->type != NODE_CALL &&
      node->param2->type != NODE_NUMBER &&
      node->param2->type != NODE_STRING &&
      node->param2->type != NODE_OPERATOR &&
      node->param2->type != NODE_CAST &&
      node->param2->type != NODE_NAME) {
    fprintf(stderr, "Generator Error: Invalid assignment expression.\n");
    exit(-1);
  }

  char *param2 = generate(node->param2);

  int param1_len = strlen(param1);
  int param2_len = strlen(param2);

  char *output = malloc(param1_len + param2_len + 4);
  size_t offset = 0;

  append(output, &offset, param1);
  output[offset++] = ' ';
  output[offset++] = '=';
  output[offset++] = ' ';
  append(output, &offset, param2);
  output[offset++] = '\0';

  free(param1);
  free(param2);

  return output;
}
