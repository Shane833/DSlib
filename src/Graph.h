#ifndef graph_h
#define graph_h

#include <list.h>
#include <darray.h>
#include <dbg.h>

// Representing graph as adjacency list
typedef struct{
    DArray *vertices;
}Graph;

Graph *Graph_create(size_t n, int nodes[n][n]);
Graph *Graph_transpose(Graph *g); 
void Graph_destroy(Graph *g);

#define Graph_count(G) DArray_count(G->vertices)

#endif
