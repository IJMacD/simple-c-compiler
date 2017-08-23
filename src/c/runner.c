#include <stdio.h>
#include <stdlib.h>

void runner(const char *output, int retain) {
  FILE *f = fopen("output.c", "w");
  fprintf(f, "%s", output);
  fclose(f);

#ifdef linux
  system("clang output.c -o output && ./output && rm output");
#endif

  if (!(retain)) {
    remove("output.c");
  }
}
