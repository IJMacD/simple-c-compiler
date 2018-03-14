#include <stdio.h>
#include <stdlib.h>

void runner(const char *output, int retain) {
  FILE *f = fopen("output.il", "w");
  fprintf(f, "%s", output);
  fclose(f);

#ifdef linux
  system("ilasm output.il -quiet -out:output.exe && mono output.exe && rm output.exe");
#else
  system("ilasm output.il /quiet /out:output.exe && ./output.exe && del output.exe");
#endif

  if (!(retain)) {
    remove("output.il");
  }
}
