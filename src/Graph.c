#include <Graph.h>

Graph *Graph_create(size_t n, int nodes[n][n]){
    Graph *new_graph = (Graph *)malloc(sizeof(Graph));
    check(new_graph != NULL, "Failed to create graph!");
    
    new_graph->vertices = DArray_create(sizeof(List), n);
    check(new_graph->vertices != NULL, "Failed to create vertex array!");

    for(int i = 0;i < n;i++){
            List *node_list = List_create();
            check(node_list != NULL, "Failed to create vertex list!");
        for(int j = 0;j < n;j++){
            if(nodes[i][j] == 1){
                int *node_num = (int *)malloc(sizeof(int));
                check(node_num != NULL, "Failed to create vertex name!");
                *node_num = j+1; // Since j is index we add one to get the position
                                  // or in this the node number
                List_push(node_list, node_num);
            }
        }
        
        check(DArray_push(new_graph->vertices, node_list) == 0, "Failed to push list in the vertex array!")
    }

    return new_graph;
error:
    return NULL;
}

Graph *Graph_transpose(Graph *g){
    Graph *new_graph = (Graph *)malloc(sizeof(Graph));
    check(new_graph != NULL, "Failed to create graph!");

    new_graph->vertices = DArray_create(sizeof(List), Graph_count(g));
    check(new_graph->vertices != NULL, "Failed to create vertex array!");

    // Adding linked lists for each entry of the darray
    for(int i = 0;i < Graph_count(g);i++){
        List *node_list = (List *)List_create();
        check(node_list != NULL, "Failed to create new list!");
        check(DArray_push(new_graph->vertices, node_list) == 0, "Failed to push node list in darray!");
    }
    
    // Now transposing the graph
    for(int i = 0;i < Graph_count(g);i++){
        // For each vertex in the original graph we obtain its
        // adjacency list
        List *node_list = (List *)DArray_get(g->vertices, i);
        LIST_FOREACH(node_list, first, next, cur){
            // For each node in its adjacency list we push the
            // current vertex as the adjacent node in the list
            // of adjacent nodes of our current vertex
            int node_idx = *(int *)cur->value - 1;
            
            List *l = (List *)DArray_get(new_graph->vertices, node_idx);
            
            int *node_num = (int *)malloc(sizeof(int));
            check(node_num != NULL, "Failed to create node int");
            *node_num = i + 1;

            List_push(l, node_num);
        }
    }
    
    return new_graph;
error:
    return NULL;
}

void Graph_destroy(Graph *g){
    if(g){
        for(int i = 0; i < Graph_count(g);i++){
            List *l = (List *)DArray_get(g->vertices, i);
            if(l){
                List_clear_destroy(l);
                // Since the list has been deallocated 
                // we should set that entry to be NULL in darray
                // bcz we if we don't do that, the subsequent
                // function DArray_clear_destroy will call a double
                // free on the same address 
                DArray_set(g->vertices, i, NULL);
            }
        } 
        DArray_clear_destroy(g->vertices);
        g->vertices = NULL;

        free(g);
    }
}
