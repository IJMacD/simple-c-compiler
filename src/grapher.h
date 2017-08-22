#include <stdlib.h>

#include "parser.h"
#include "traverser.h"
#include "util.h"

void grapher(FILE *, ast_node *);
void graph_node(ast_node *node, char *, int *, int *, FILE *);
