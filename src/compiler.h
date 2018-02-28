#include "lexer.h"
#include "parser.h"
#include "traverser.h"
#include "transformer.h"
#include "generator.h"
#include "linker.h"
#include "executor.h"
#include "grapher.h"
#include "randomiser.h"
#include "runner.h"

#define OPTION_RETAIN     1
#define OPTION_PRINT      2
#define OPTION_VERBOSE    4
#define OPTION_TRANSFORM  8
#define OPTION_EXECUTE   16
#define OPTION_GRAPH     32
#define OPTION_COMPILE   64
#define OPTION_RANDOM   128
#define OPTION_CONSTANT_FOLDING   256

void compiler(const char*, int);
