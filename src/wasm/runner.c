#include <stdio.h>
#include <stdlib.h>

void runner(const char *output, int retain) {
  FILE *f = fopen("output.wat", "w");
  fprintf(f, "%s", output);
  fclose(f);

#ifdef linux
  system("wat2wasm output.wat -o output.wasm && wasm-interop output.wasm && rm output.wasm");
#else
  system("wat2wasm output.wat -o output.wasm && wasm-interop output.wasm && del output.wasm");
#endif

  if (!(retain)) {
    remove("output.wat");
  }
}
