#include "util.h"

#define FLAG_INCLUDE_ADD  1
#define FLAG_INCLUDE_SUB  2
#define FLAG_INCLUDE_MUL  4
#define FLAG_INCLUDE_DIV  8
#define FLAG_INCLUDE_FAC 16
#define FLAG_INCLUDE_POWER 32

#define FLAG_INCLUDE_STDIO 256
#define FLAG_INCLUDE_MATH 512

char* linker(char *, int);
