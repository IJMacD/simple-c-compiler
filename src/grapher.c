#include <stdio.h>

#include "grapher.h"

void grapher(FILE *output, ast_node *node) {
    int operator_count = 0;
    int number_count = 0;

    graph_node(node, "calc", &operator_count, &number_count, output);
}

void graph_node (ast_node *node, char *parent_id, int *operator_count, int *number_count, FILE *output) {
    if(node == NULL) {
        fprintf(stderr, "Grapher Error: invalid node.\n");
        exit(-1);
    }

    switch (node->type) {
        case NODE_PROGRAM:
            fprintf(output, "graph %s {\n", parent_id);
            graph_node(node->body[0], NULL, operator_count, number_count, output);
            fprintf(output, "}\n");
            break;
        case NODE_OPERATOR:
            fprintf(output, "\tO%d [label=\"%s\"]\n", *operator_count, node->string_val);

            if (parent_id != NULL) {
                // If no parent is passed in then we are the root node
                fprintf(output, "\t%s -- O%d\n", parent_id, *operator_count);
            }

            char tmp_buff[255];
            sprintf(tmp_buff, "O%d", *operator_count);

            (*operator_count)++;

            if (node->param1 != NULL)
                graph_node(node->param1, tmp_buff, operator_count, number_count, output);
            if (node->param2 != NULL)
                graph_node(node->param2, tmp_buff, operator_count, number_count, output);
            break;
        case NODE_NUMBER:
            fprintf(output, "\tN%d [label=%ld]\n", *number_count, node->int_val);

            if (parent_id != NULL) {
                fprintf(output, "\t%s -- N%d\n", parent_id, *number_count);
            }

            (*number_count)++;
            break;
    }
}
