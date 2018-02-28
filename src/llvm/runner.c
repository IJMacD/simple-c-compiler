#include <stdio.h>
#include <stdlib.h>

void runner(const char *output, int retain) {
  FILE *f = fopen("output.ll", "w");
  fprintf(f, "%s", output);
  fclose(f);

#ifdef linux
  system("llvm-as output.ll && lli output.bc && rm output.bc");
#else
  system("llvm-as output.ll && lli output.bc && del output.bc");
#endif

  if (!(retain)) {
    remove("output.ll");
  }
}
