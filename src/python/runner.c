#include <stdio.h>
#include <stdlib.h>

#include "../common/runner.h"

void runner(const char *output, int retain) {
  FILE *f = fopen("output.py", "w");
  fprintf(f, "%s", output);
  fclose(f);

  system("python output.py");

  if (!(retain)) {
    remove("output.py");
  }
}
