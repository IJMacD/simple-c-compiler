#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/linker.h"

char* linker(char *program, int include_flags) {

  static const char include_fac[] = "def factorial(a):\n\tb = 1\n\twhile(a > 0):\n\t\tb *= a\n\t\ta -= 1\n\treturn b\n";

  int output_len = strlen(program) + 1;

  if(include_flags) {
    output_len += 1; // One additional '\n'
  }

  if(include_flags & FLAG_INCLUDE_FAC) {
    output_len += sizeof(include_fac) - 1;
  }

  char *output = malloc(output_len);
  size_t offset = 0;

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
