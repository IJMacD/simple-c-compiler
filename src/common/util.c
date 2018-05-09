#include <string.h>

#include "util.h"

size_t append(char *dest, size_t *offset, char const *source) {
  size_t size = strlen(source);
  memcpy(&dest[*offset], source, size);
  (*offset) += size;
  return size;
}
