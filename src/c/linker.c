#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../linker.h"

char* linker(char *program, int include_flags) {

  static const char include_stdio[] = "#include <stdio.h>\n";
  static const char include_power[] = "int power(int a, int b) { int r = 1; while(b--) r *= a; return r; }\n";
  static const char include_add[] = "int add(int a, int b) { return a + b; }\n";
  static const char include_sub[] = "int subtract(int a, int b) { return a - b; }\n";
  static const char include_mul[] = "int multiply(int a, int b) { return a * b; }\n";
  static const char include_div[] = "int divide(int a, int b) { return a / b; }\n";
  static const char include_fac[] = "int factorial(int a) { int b = 1; while(a > 0) b *= a--; return b; }\n";

  int output_len = strlen(program) + 1;

  if(include_flags & FLAG_INCLUDE_STDIO) {
    output_len += sizeof(include_stdio) - 1;
  }

  if(include_flags & FLAG_INCLUDE_POWER) {
    output_len += sizeof(include_power) - 1;
  }

  if(include_flags) {
    output_len += 1; // One additional '\n'
  }

  if(include_flags & FLAG_INCLUDE_ADD) {
    output_len += sizeof(include_add) - 1;
  }

  if(include_flags & FLAG_INCLUDE_SUB) {
    output_len += sizeof(include_sub) - 1;
  }

  if(include_flags & FLAG_INCLUDE_MUL) {
    output_len += sizeof(include_mul) - 1;
  }

  if(include_flags & FLAG_INCLUDE_DIV) {
    output_len += sizeof(include_div) - 1;
  }

  if(include_flags & FLAG_INCLUDE_FAC) {
    output_len += sizeof(include_fac) - 1;
  }

  char *output = malloc(output_len);
  int offset = 0;

  if(include_flags & FLAG_INCLUDE_STDIO) {
    append(output, &offset, include_stdio);
  }

  if(include_flags & FLAG_INCLUDE_POWER) {
    append(output, &offset, include_power);
  }

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

  if(include_flags & FLAG_INCLUDE_FAC) {
    append(output, &offset, include_fac);
  }

  if(include_flags) {
    output[offset++] = '\n';
  }

  append(output, &offset, program);

  output[offset] = '\0';

  return output;
}
