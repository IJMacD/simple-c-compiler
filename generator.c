#include <stdio.h>
#include <string.h>

char* generator(ast_node *, int *);
char* generate(ast_node *);
char* generate_program(ast_node *);
char* generate_statement(ast_node *);
char* generate_call(ast_node *);
char* generate_number(ast_node *);
char* generate_string(ast_node *);
char* generate_operator(ast_node *);

int append(char *, int *, char const *);

#define FLAG_INCLUDE_ADD 1
#define FLAG_INCLUDE_SUB 2
#define FLAG_INCLUDE_MUL 4
#define FLAG_INCLUDE_DIV 8

#define FLAG_INCLUDE_STDIO 32

int *include_flags;

char* generator(ast_node *node, int *flags) {
  include_flags = flags;

  return generate(node);
}

char* generate(ast_node *node) {
  if(node == NULL) {
    printf("Generator Error: Missing Node.\n");
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

  printf("Generator Error: Unrecognised Node Type: %d\n", node->type);
  exit(-1);
}

char* generate_program(ast_node *node) {
  static const char head[] = "int main(){\n";
  static const char tail[] = "}\n";

  static const int head_len = sizeof(head) - 1;
  static const int tail_len = sizeof(tail) - 1;

  char body[1024];
  int offset = 0;
  int  i;

  for(i = 0; i < node->body_length; i++) {
    if(node->body[i] != NULL){
      char *child = generate(node->body[i]);
      int len = strlen(child);
      if(offset + len > 1024) {
        printf("Generator Error: Not enough space reserved for body.");
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
  int offset = 0;

  append(output, &offset, body);
  output[offset++] = ';';
  output[offset++] = '\n';
  output[offset++] = '\0';

  free(body);

  return output;
}

char* generate_call (ast_node *node) {
  if(node->param1 == NULL) {
    printf("Generator Error: `%s` Missing first paramater.\n", node->string_val);
    exit(-1);
  }
  char *param1 = generate(node->param1);

  if(node->param2 == NULL) {
    printf("Generator Error: `%s` Missing second paramater.\n", node->string_val);
    exit(-1);
  }
  char *param2 = generate(node->param2);

  int name_len = strlen(node->string_val);
  int param1_len = strlen(param1);
  int param2_len = strlen(param2);

  if(!strcmp("add", node->string_val)){
    *include_flags |= FLAG_INCLUDE_ADD;
  } else if (!strcmp("subtract", node->string_val)) {
    *include_flags |= FLAG_INCLUDE_SUB;
  } else if (!strcmp("multiply", node->string_val)) {
    *include_flags |= FLAG_INCLUDE_MUL;
  } else if (!strcmp("divide", node->string_val)) {
    *include_flags |= FLAG_INCLUDE_DIV;
  } else if (!strcmp("printf", node->string_val)) {
    *include_flags |= FLAG_INCLUDE_STDIO;
  } else {
    printf("Generator Error: Unrecognised call target `%s`.\n", node->string_val);
    exit(1);
  }

  char *output = malloc(name_len + param1_len + param2_len + 5);
  int offset = 0;

  append(output, &offset, node->string_val);
  output[offset++] = '(';
  append(output, &offset, param1);
  output[offset++] = ',';
  output[offset++] = ' ';
  append(output, &offset, param2);
  output[offset++] = ')';
  output[offset++] = '\0';

  free(param1);
  free(param2);

  return output;
}

char* generate_number(ast_node *node) {
  char *output = calloc(32, sizeof(char));
  sprintf(output, "%d", node->int_val);
  return output;
}

char* generate_string(ast_node *node) {
  int string_len = strlen(node->string_val);
  char *output = malloc(string_len + 3);

  output[0] = '"';
  memcpy(&output[1], node->string_val, string_len);
  output[string_len + 1] = '"';
  output[string_len + 2] = '\0';

  return output;
}

char* generate_operator (ast_node *node) {
  if(node->param1 == NULL) {
    printf("Generator Error: `%s` Missing first operand.\n", node->string_val);
    exit(-1);
  }
  char *param1 = generate(node->param1);

  if(node->param2 == NULL) {
    printf("Generator Error: `%s` Missing second operand.\n", node->string_val);
    exit(-1);
  }
  char *param2 = generate(node->param2);

  int param1_len = strlen(param1);
  int param2_len = strlen(param2);

  char *output = malloc(param1_len + param2_len + 6);
  int offset = 0;

  output[offset++] = '(';
  append(output, &offset, param1);
  output[offset++] = ' ';
  append(output, &offset, node->string_val);
  output[offset++] = ' ';
  append(output, &offset, param2);
  output[offset++] = ')';
  output[offset++] = '\0';

  free(param1);
  free(param2);

  return output;
}

int append(char *dest, int *offset, char const *source) {
  int size = strlen(source);
  memcpy(&dest[*offset], source, size);
  (*offset) += size;
  return size;
}
