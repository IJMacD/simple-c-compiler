#include <stdio.h>
#include <string.h>

char* generate(ast_node *);
char* generate_program(ast_node *);
char* generate_call(ast_node *);
char* generate_number(ast_node *);
char* generate_string(ast_node *);

int append(char *, int *, char const *);

#define FLAG_INCLUDE_ADD 1
#define FLAG_INCLUDE_SUBTRACT 2

int include_flags = 0;

char* generate(ast_node *node) {
  if(node == NULL) {
    printf("Generator Error: Missing Node.\n");
    exit(-1);
  }

  if(node->type == NODE_PROGRAM) {
    return generate_program(node);
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

  return NULL;
}

char* generate_program(ast_node *node) {
  char head[] = "#include <stdio.h>\n\nint add(int, int);\nint subtract(int, int);\n\nint main(){\n\tint result = ";
  char tail[] = ";\n\tprintf(\"%d\\n\", result);\n}\n";
  char include_add[] = "int add(int a, int b) { return a + b; }\n";
  char include_subtract[] = "int subtract(int a, int b) { return a - b; }\n";

  char *body;

  if(node->param1 != NULL){
    body = generate(node->param1);
  }

  const int head_len = strlen(head);
  const int body_len = strlen(body);
  const int tail_len = strlen(tail);
  const int include_add_len = strlen(include_add);
  const int include_subtract_len = strlen(include_subtract);

  int output_len = head_len + body_len + tail_len + 1;

  if(include_flags & FLAG_INCLUDE_ADD) {
    output_len += include_add_len;
  }

  if(include_flags & FLAG_INCLUDE_SUBTRACT) {
    output_len += include_subtract_len;
  }

  char *output = malloc(output_len);
  int offset = 0;

  append(output, &offset, head);
  append(output, &offset, body);
  append(output, &offset, tail);

  if(include_flags & FLAG_INCLUDE_ADD) {
    append(output, &offset, include_add);
  }

  if(include_flags & FLAG_INCLUDE_SUBTRACT) {
    append(output, &offset, include_subtract);
  }

  output[offset] = '\0';

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
    include_flags |= FLAG_INCLUDE_ADD;
  } else if (!strcmp("subtract", node->string_val)) {
    include_flags |= FLAG_INCLUDE_SUBTRACT;
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

int append(char *dest, int *offset, char const *source) {
  int size = strlen(source);
  memcpy(&dest[*offset], source, size);
  (*offset) += size;
  return size;
}
