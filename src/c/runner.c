#include <stdio.h>
#include <stdlib.h>

#include "../common/runner.h"

void runner(const char *output, int retain) {
  FILE *f = fopen("output.c", "w");
  fprintf(f, "%s", output);
  fclose(f);

#ifdef linux
  system("clang output.c -lm -o output && ./output && rm output");
#else
  system("clang output.c -o output.exe && .\\output.exe && del output.exe");
#endif

  if (!(retain)) {
    remove("output.c");
  }
}
