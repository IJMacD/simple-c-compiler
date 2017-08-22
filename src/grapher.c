#include <stdio.h>

#include "grapher.h"

int operator_count = 0;
int number_count = 0;

void grapher(ast_node *node, FILE *output) {
    graph_node(node, "calc", output);
}

void graph_node (ast_node *node, char *parent_id, FILE *output) {
    if(node == NULL) {
        fprintf(stderr, "Grapher Error: invalid node.\n");
        exit(-1);
    }

    switch (node->type) {
        case NODE_PROGRAM:
            fprintf(output, "graph %s {\n", parent_id);
            graph_node(node->body[0], NULL, output);
            fprintf(output, "}\n");
            break;
        case NODE_OPERATOR:
            fprintf(output, "\tO%d [label=\"%s\"]\n", operator_count, node->string_val);

            if (parent_id != NULL) {
                // If no parent is passed in then we are the root node
                fprintf(output, "\t%s -- O%d\n", parent_id, operator_count);
            }

            char tmp_buff[255];
            sprintf(tmp_buff, "O%d", operator_count);

            operator_count++;

            graph_node(node->param1, tmp_buff, output);
            graph_node(node->param2, tmp_buff, output);
            break;
        case NODE_NUMBER:
            fprintf(output, "\tN%d [label=%d]\n", number_count, node->int_val);

            if (parent_id != NULL) {
                fprintf(output, "\t%s -- N%d\n", parent_id, number_count);
            }

            number_count++;
            break;
    }
}
