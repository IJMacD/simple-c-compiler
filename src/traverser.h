#include "parser.h"

ast_node *traverser(ast_node *, ast_node *(*visitor) (ast_node *, ast_node *));
ast_node *traverse_node(ast_node *, ast_node *, ast_node *(*visitor) (ast_node *, ast_node *));
