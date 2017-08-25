#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../linker.h"

char* linker(char *program, int include_flags) {

  static const char head[] = "class Stdlib {\n";
  static const char include_fac[] = "\tpublic static int factorial(int a){int b=1;while(a>0)b*=a--;return b;}\n";

  int output_len = strlen(program) + sizeof(head) + 1;

  if(include_flags) {
    output_len += 1; // One additional '\n'
  }

  if(include_flags & FLAG_INCLUDE_FAC) {
    output_len += sizeof(include_fac) - 1;
  }

  char *output = malloc(output_len);
  int offset = 0;


  append(output, &offset, program);

  if(include_flags) {
    output[offset++] = '\n';
    append(output, &offset, head);
  }

  if(include_flags & FLAG_INCLUDE_FAC) {
    append(output, &offset, include_fac);
  }

  if(include_flags) {
    output[offset++] = '}';
    output[offset++] = '\n';
  }

  output[offset++] = '\0';

  return output;
}
