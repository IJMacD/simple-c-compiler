#include <stdlib.h>

#include "parser.h"

#define RANDOMISER_NUM_MIN   -100
#define RANDOMISER_NUM_MAX    100
#define RANDOMISER_NODE_COUNT 100

ast_node *randomiser();
ast_node *random_operator();
ast_node *random_number();
