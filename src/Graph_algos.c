#include <Graph_algos.h>

static inline int asc_order(int a, int b){
   return a - b; 
}

static inline int desc_order(int a, int b){
    return b - a;
}

// Global Enum of Colors used in algorithms
// The members of the enum are directly
// available without needing to create 
// an object of type Color
enum Color{
        WHITE,
        GRAY,
        BLACK
};

void Graph_BFS(Graph *g, int src_vertex){
// YOU LEARN SOMETHING NEW EVERYDAY
   // check(g != NULL, "Invalid Graph!"); This is in error bcz the arrays declared below
   // are dynamic/modifiable in size and the program needs to know that first, as
   // it has to allocate memory on the stack, therefore with this "goto error"
   // there is a possibility that it may directly jump to the return statement
   // and the arrays can't be created properly
   // SOLUTION : One solution to this problem can be that we provide the size
   // of the arrays to be declared separately as an argument to the function
   // this would remove the dependence from Graph object 'g'

   // Storing each of the attributes in a separate array
    const int vertex_count = Graph_count(g);
    int vertex_color[vertex_count];
    float vertex_distance[vertex_count];
    int vertex_parent[vertex_count];
    
    check(g != NULL, "Invalid Graph!");

    // Initialization of all vertices except our src vertex
    // Setting up all of the vertices to be white i.e. unvisited
    // distance from source to be infinity
    // and each having no parent

    int src_vertex_idx = src_vertex - 1;

    for(int i = 0;i < Graph_count(g);i++){
        if(i == src_vertex_idx) continue;
        vertex_color[i] = WHITE;
        vertex_distance[i] = INFINITY;
        vertex_parent[i] = 0; // 0 is equivalent of NIL
    }
    
    // Initializing our src vertex
    vertex_color[src_vertex_idx] = GRAY;
    vertex_distance[src_vertex_idx] = 0;
    vertex_parent[src_vertex_idx] = 0;
    
    Queue *q = Queue_create();
    check(q != NULL, "Failed to create the queue!");
    
    // Pushing our source vertex into the queue
    int *src = (int *)malloc(sizeof(int));
    check(src != NULL, "Failed to allocate src node!");
    *src = src_vertex;

    Queue_enqueue(q, src);
    
    // Until our queue becomes empty we will keep on visiting nodes
    while(Queue_count(q) > 0){
        int u_idx = *(int *)Queue_dequeue(q) - 1; // Since the queue contains the node num
                                                  // we convert it to index
        // Printing our BFS path
        printf("%d ", u_idx + 1);
        
        List *u_adj_list = (List *)DArray_get(g->vertices, u_idx);
        // Going through all the neighbors of u
        LIST_FOREACH(u_adj_list, first, next, cur){
            // cur->value contains the graph node
            int cur_vertex_idx = *(int *)cur->value - 1;

            if(vertex_color[cur_vertex_idx] == WHITE){
                vertex_color[cur_vertex_idx] = GRAY;
                vertex_distance[cur_vertex_idx] = vertex_distance[u_idx] + 1;
                vertex_parent[cur_vertex_idx] = u_idx + 1;

                int *cur_node = (int *)malloc(sizeof(int));
                check(cur_node != NULL, "Failed to allocate cur node!");
                *cur_node = *(int *)cur->value;

                Queue_enqueue(q, cur_node);
            }
        }
        vertex_color[u_idx] = BLACK;
    }
    
    Queue_clear_destroy(q);

error:
    return;
}

// Global time variable for keeping track of discovery and finishing time
static int time = 0;

// Utitlity function for visiting the adjacent nodes of 
// each newly discovered vertex in the DFS algorithm
static void DFS_visit(Graph *g, int vertex_dtime[], int vertex_ftime[], int vertex_color[], int vertex_parent[], int src_vertex){
    // Printing the order of DFS
    printf("%d ", src_vertex);
    int src_idx = src_vertex - 1;
    // increment time as we have discovered a new vertex
    time++;
    vertex_dtime[src_idx] = time;
    vertex_color[src_idx] = GRAY; // Paint it gray as it has been discovered now
    // Looping over the adjacent nodes of the src vertex
    List *src_adj_list = (List *)DArray_get(g->vertices, src_idx);
    LIST_FOREACH(src_adj_list, first, next, cur){
        int cur_vertex_idx = *(int *)cur->value - 1;
        if(vertex_color[cur_vertex_idx] == WHITE){
            vertex_parent[cur_vertex_idx] = src_vertex;
            DFS_visit(g, vertex_dtime, vertex_ftime, vertex_color, vertex_parent, cur_vertex_idx + 1); // Exploring this newly discovered vertex
        }
    }
    
    time++; // increment time after exhausting all the vertices of our source vertex
    vertex_ftime[src_idx] = time; // Setting this as the finishing time
    vertex_color[src_idx] = BLACK;
}

void Graph_DFS(Graph *g, int vertex_dtime[], int vertex_ftime[]){
    // Declaring the attribute array for each vertex
    const int vertex_count = Graph_count(g);
    int vertex_color[vertex_count];
    int vertex_parent[vertex_count];

    // Initialization
    for(int i = 0;i < Graph_count(g);i++){
        vertex_color[i] = WHITE;
        vertex_parent[i] = 0; // 0 is equivalent of NIL
        vertex_dtime[i] = 0;
        vertex_ftime[i] = 0;
    }
    time = 0; // Setting gloabl time to be zero
    
    for(int i = 0;i < Graph_count(g);i++){
        if(vertex_color[i] == WHITE){
            DFS_visit(g, vertex_dtime, vertex_ftime, vertex_color, vertex_parent, i+1); // Recursively visiting all the adjacent
                                                                                       // nodes of 'i+1'th vertex
        }
    }
    printf("\n");
}

void Graph_SortVertices(size_t n, int to_sort[n], int vertices[n], Compare cmp){
// Insertion Sort
   for(int i = 1;i < n;i++){
        int key = to_sort[i];
        int key2 = vertices[i];
        int j = i - 1;
        while(j >= 0 && cmp(key, to_sort[j]) < 0){
            to_sort[j+1] = to_sort[j]; // Shift the elements to make space
            vertices[j+1] = vertices[j];
            j--;
        }
        // Now simply place the element at its correct place
        to_sort[j+1] = key;
        vertices[j+1] = key2;
   }
}

void Graph_TopologicalSort(Graph *g, List *result){
   int v_dtime[Graph_count(g)]; 
   int v_ftime[Graph_count(g)]; 

   Graph_DFS(g, v_dtime, v_ftime); // Calling DFS on the graph

   // Sort in descending order of finishing times
   int vertices[Graph_count(g)];
   for(int i = 0;i < Graph_count(g);i++){
       vertices[i] = i + 1;
   }
   // Applying insertion sort
   Graph_SortVertices(Graph_count(g), v_ftime, vertices, asc_order); 
   
   // Inserting these into a linked list
   for(int i = 0;i < Graph_count(g);i++){
       int *node = (int *)malloc(sizeof(int));
       *node = vertices[i];
       List_unshift(result, node); // Inserting at the beginning of the list 
   }
}

void Graph_SSComponents(Graph *g, int v_dtime[], int v_ftime[]){

    Graph_DFS(g, v_dtime, v_ftime); // We perform simply DFS first on our graph

    // Arrange vertices in decreasing order of finishing time
    int vertices[Graph_count(g)];
    for(int i = 0;i < Graph_count(g);i++){
        vertices[i] = i + 1;
    }
    // Applying insertion sort
    Graph_SortVertices(Graph_count(g), v_ftime, vertices, desc_order);
    
    // Create transpose graph
    Graph *t = Graph_transpose(g);

    // Applying DFS on the transposed graph but feed vertices in
    // decreasing order of their original finishing times
    int vertex_color[Graph_count(g)];
    int vertex_parent[Graph_count(g)];

    // Initialization
    for(int i = 0;i < Graph_count(g);i++){
        vertex_color[i] = WHITE;
        vertex_parent[i] = 0; // 0 is equivalent of NIL
        v_dtime[i] = 0;
        v_ftime[i] = 0;
    }

    time = 0; // Setting global time to be zero
    
    for(int i = 0;i < Graph_count(g);i++){
        int vertex_idx = vertices[i] - 1;
        if(vertex_color[vertex_idx] == WHITE){
            DFS_visit(t, v_dtime, v_ftime, vertex_color, vertex_parent, vertices[i]); 
              // Recursively visiting all the adjacent of the given vertex            
        }       
    }
    printf("\n");

    Graph_destroy(t);
    t = NULL;
}
