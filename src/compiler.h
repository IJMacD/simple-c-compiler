#include "lexer.h"
#include "parser.h"
#include "traverser.h"
#include "transformer.h"
#include "generator.h"
#include "linker.h"
#include "executor.h"

#define OPTION_RETAIN     1
#define OPTION_PRINT      2
#define OPTION_VERBOSE    4
#define OPTION_TRANSFORM  8
#define OPTION_EXECUTE   16

void compiler(const char*, int);
ast_node* operator_switcher(ast_node *, ast_node *);
