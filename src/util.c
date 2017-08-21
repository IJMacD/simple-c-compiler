#include <string.h>

#include "util.h"

int append(char *dest, int *offset, char const *source) {
  int size = strlen(source);
  memcpy(&dest[*offset], source, size);
  (*offset) += size;
  return size;
}
