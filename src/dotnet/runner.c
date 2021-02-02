#include <stdio.h>
#include <stdlib.h>

#include "../common/runner.h"

char *proj = "<Project Sdk=\"Microsoft.NET.Sdk\">\n"
"\n"
"  <PropertyGroup>\n"
"    <OutputType>Exe</OutputType>\n"
"    <TargetFramework>net5.0</TargetFramework>\n"
"    <RootNamespace>simple_calc_compiler</RootNamespace>\n"
"  </PropertyGroup>\n"
"\n"
"</Project>\n";

void runner(const char *output, int retain) {
  FILE *f1 = fopen("output.csproj", "w");
  fprintf(f1, "%s", proj);
  fclose(f1);

  FILE *f = fopen("output.cs", "w");
  fprintf(f, "%s", output);
  fclose(f);

  system("dotnet run");

  if (!(retain)) {
    remove("output.cs");
    remove("output.csproj");
  }
}


// void runner(const char *output, int retain) {
//   FILE *f = fopen("output.csx", "w");
//   fprintf(f, "%s", output);
//   fclose(f);

//   system("dotnet-script output.csx");

//   if (!(retain)) {
//     remove("output.csx");
//   }
// }
