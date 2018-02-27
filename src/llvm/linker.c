#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../linker.h"

char* linker(char *program, int include_flags) {

  static const char include_stdio[] = "#include <stdio.h>\n";
  static const char include_math[] = "#include <math.h>\n";
  static const char include_add[] = "long add(long a, long b) { return a + b; }\n";
  static const char include_sub[] = "long subtract(long a, long b) { return a - b; }\n";
  static const char include_mul[] = "long multiply(long a, long b) { return a * b; }\n";
  static const char include_div[] = "long divide(long a, long b) { return a / b; }\n";
  static const char include_fac[] = "long factorial(long a) { long b = 1; while(a > 0) b *= a--; return b; }\n";
  static const char include_power[] = "long power(long a, long b) { long r = 1; while(b--) r *= a; return r; }\n";

  int output_len = strlen(program) + 1;

  if(include_flags & FLAG_INCLUDE_STDIO) {
    output_len += sizeof(include_stdio) - 1;
  }

  if(include_flags & FLAG_INCLUDE_MATH) {
    output_len += sizeof(include_math) - 1;
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

  if(include_flags & FLAG_INCLUDE_POWER) {
    output_len += sizeof(include_power) - 1;
  }

  char *output = malloc(output_len);
  size_t offset = 0;

  if(include_flags & FLAG_INCLUDE_STDIO) {
    append(output, &offset, include_stdio);
  }

  if(include_flags & FLAG_INCLUDE_MATH) {
    append(output, &offset, include_math);
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

  if(include_flags & FLAG_INCLUDE_POWER) {
    append(output, &offset, include_power);
  }

  if(include_flags) {
    output[offset++] = '\n';
  }

  append(output, &offset, program);

  output[offset] = '\0';

  return output;
}
