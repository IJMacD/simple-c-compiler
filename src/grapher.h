#include <stdlib.h>

#include "parser.h"
#include "traverser.h"
#include "util.h"

void grapher(ast_node *, FILE *);
void graph_node(ast_node *node, char *, FILE *);
