#include "parser.h"
#include "linker.h"
#include "util.h"

char* generator(ast_node *, int *);
char* generate(ast_node *);
char* generate_program(ast_node *);
char* generate_statement(ast_node *);
char* generate_call(ast_node *);
char* generate_number(ast_node *);
char* generate_string(ast_node *);
char* generate_operator(ast_node *);
