#include <stdio.h>

char* linker(char *, int);

char* linker(char *program, int include_flags) {

  static const char head[] = "#include <stdio.h>\n\n";
  static const char include_add[] = "int add(int a, int b) { return a + b; }\n";
  static const char include_sub[] = "int subtract(int a, int b) { return a - b; }\n";
  static const char include_mul[] = "int multiply(int a, int b) { return a * b; }\n";
  static const char include_div[] = "int divide(int a, int b) { return a / b; }\n";

  static const int head_len = sizeof(head) - 1;
  static const int include_add_len = sizeof(include_add) - 1;
  static const int include_sub_len = sizeof(include_sub) - 1;
  static const int include_mul_len = sizeof(include_mul) - 1;
  static const int include_div_len = sizeof(include_div) - 1;

  int output_len = head_len + strlen(program) + 1;

  if(include_flags) {
    output_len += 1; // One additional '\n'
  }

  if(include_flags & FLAG_INCLUDE_ADD) {
    output_len += include_add_len;
  }

  if(include_flags & FLAG_INCLUDE_SUB) {
    output_len += include_sub_len;
  }

  if(include_flags & FLAG_INCLUDE_MUL) {
    output_len += include_mul_len;
  }

  if(include_flags & FLAG_INCLUDE_DIV) {
    output_len += include_div_len;
  }

  char *output = malloc(output_len);
  int offset = 0;

  append(output, &offset, head);

  if(include_flags & FLAG_INCLUDE_ADD) {
    append(output, &offset, include_add);
  }

  if(include_flags & FLAG_INCLUDE_SUB) {
    append(output, &offset, include_sub);
  }

  if(include_flags & FLAG_INCLUDE_MUL) {
    append(output, &offset, include_mul);
  }

  if(include_flags & FLAG_INCLUDE_DIV) {
    append(output, &offset, include_div);
  }

  if(include_flags) {
    output[offset++] = '\n';
  }

  append(output, &offset, program);

  output[offset] = '\0';

  return output;
}
