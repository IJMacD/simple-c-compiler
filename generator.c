#include <stdio.h>
#include <string.h>

char* generate(ast_node *);
char* generate_program(ast_node *);
char* generate_call(ast_node *);
char* generate_number(ast_node *);
char* generate_string(ast_node *);

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
  char tail[] = ";\n\tprintf(\"%d\\n\", result);\n}\n\nint add(int a, int b) { return a + b; }\nint subtract(int a, int b) { return a - b; }\n";

  char *body;

  if(node->param1 != NULL){
    body = generate(node->param1);
  }

  int head_len = strlen(head);
  int body_len = strlen(body);
  int tail_len = strlen(tail);
  char *output = malloc(head_len + body_len + tail_len + 1);

  memcpy(output, head, head_len);
  memcpy(&output[head_len], body, body_len);
  memcpy(&output[head_len + body_len], tail, tail_len);
  output[head_len + body_len + tail_len] = '\0';

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

  char *output = malloc(name_len + param1_len + param2_len + 5);

  memcpy(output, node->string_val, name_len);
  output[name_len] = '(';
  memcpy(&output[name_len + 1], param1, param1_len);
  output[name_len + param1_len + 1] = ',';
  output[name_len + param1_len + 2] = ' ';
  memcpy(&output[name_len + param1_len + 3], param2, param2_len);
  output[name_len + param1_len + param2_len + 3] = ')';

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
