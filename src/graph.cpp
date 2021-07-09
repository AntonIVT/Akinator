#include "graph.h"
#include <assert.h>
#include <stdlib.h>

void GetGraph(Tree *tree)
{
    assert(tree != nullptr);
    
    FILE *graph_file = fopen("Graph.gv", "wb");
    fprintf(graph_file, "digraph structs {\n"
                        "    node [shape=record];\n");
        
    GraphNode(graph_file, tree->root);
    
    fprintf(graph_file, "}\n");
    fclose(graph_file);
    
    system("dot -Tpdf Graph.gv -o Graph_Akinator.pdf && rm Graph.gv");
    
    printf("Done! You can check Graph_Akinator.pdf\n");
}

void GraphNode(FILE* graph_file, Node *node)
{
    if (node == nullptr)
        return;
    
    fprintf(graph_file, "struct_%p [shape=record, style = filled, fillcolor = \"#8ddfff\", label = \"  %s \"];\n", node, node->line);
        
    if (node->left != nullptr)
    {
        GraphNode(graph_file, node->left);
        fprintf(graph_file, "edge [color = \"#ff9d14\"];\n struct_%p->struct_%p [style = bold,label = \"Yes\"];\n", node, node->left);
    }
    if (node->right != nullptr)
    {
        GraphNode(graph_file, node->right);
        fprintf(graph_file, "edge [color = \"#ff5533\"];\nstruct_%p->struct_%p [style = bold,label = \"No\"];\n", node, node->right);
    }
    return;
}
