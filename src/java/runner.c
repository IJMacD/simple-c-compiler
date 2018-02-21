#include <stdio.h>
#include <stdlib.h>

void runner(const char *output, int retain) {
  FILE *f = fopen("output.java", "w");
  fprintf(f, "%s", output);
  fclose(f);

#ifdef linux
  system("javac output.java && java Program && rm Program.class");
#else
  system("javac output.java && java Program && del Program.class Stdlib.class");
#endif

  if (!(retain)) {
    remove("output.java");
  }
}
