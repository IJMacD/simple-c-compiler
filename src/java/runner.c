#include <stdio.h>
#include <stdlib.h>

void runner(const char *output, int retain) {
  FILE *f = fopen("output.java", "w");
  fprintf(f, "%s", output);
  fclose(f);

  system("javac output.java && java Program.class && rm Program.class");

  if (!(retain)) {
    remove("output.java");
  }
}
