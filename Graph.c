/********************************************************************************* 
* Diwa Ashwini Vittala, dashwini
* 2022 Spring CSE101 PA3
* Graph.c
* Graph ADT 
**********************************************************************************/

#include "Graph.h"
#include "List.h"

// A graph structure
typedef struct GraphObj {
    int size; // number of arcs or edges
    int vertices; // total number of vertices in the graph
    char* color; // determine if a vertex has been visted or not 
                 // (w = undiscovered, g = discorvered but not neighbors, b = done)
    List* neighbors; // the graph, made up of lists
    int* parent; // contains all the parents of each vertex
    int* discover; // the distance to the most recent source to vertex i
    int* finish;
    int time;
} GraphObj;

/*** Constructors-Destructors ***/
// newGraph(): create a new graph
Graph newGraph(int n) {
    Graph G = malloc(sizeof(GraphObj));
    G->vertices = n;
    G->size = 0;
    G->time = 0;

    // start with color = w, distance = inf, parent = nil
    G->color = (char *) calloc((n + 1), sizeof(char));
    G->neighbors = (List *) calloc((n + 1), sizeof(List));
    G->parent = (int *) calloc((n + 1), sizeof(int));
    G->discover = (int *) calloc((n + 1), sizeof(int));
    G->finish = (int *) calloc((n + 1), sizeof(int));

    for (int i = 0; i <= n; i += 1) {
        G->neighbors[i] = newList();
        G->color[i] = 'w';
        G->parent[i] = NIL;
        G->discover[i] = UNDEF;
        G->finish[i] = UNDEF;
    }

    return G;
}

// freeGraph(): destroys a graph
void freeGraph(Graph* pG) {
    if (pG && *pG) {
        for (int i = 0; i <= (*pG)->vertices; i += 1) {
            freeList(&(((*pG)->neighbors)[i]));
        }
        free(((*pG)->neighbors));
        if ((*pG)->parent) {
            free((*pG)->parent);
            (*pG)->parent = NULL;
        }
        if ((*pG)->discover) {
            free((*pG)->discover);
            (*pG)->discover = NULL;
        }
        if ((*pG)->color) {
            free((*pG)->color);
            (*pG)->color = NULL;
        }
        if ((*pG)->finish) {
            free((*pG)->finish);
            (*pG)->finish = NULL;
        }
        free(*pG);
        *pG = NULL;
    }
    return;
}

/*** Access functions ***/
// getOrder(): returns number of vertices in graph
int getOrder(Graph G) {
    if (G) {
        return G->vertices;
    } else {
        printf("Graph error: calling getOrder() on non existent graph.\n");
        exit(EXIT_FAILURE);
    }
}

// getSize(): returns number of edges or arcs in graph
int getSize(Graph G) {
    if (G) {
        return G->size;
    } else {
        printf("Graph error: calling getSize() on non existent graph.\n");
        exit(EXIT_FAILURE);
    }
}

// getParent(): returns the parent of a vertex
int getParent(Graph G, int u) {
    if ((u >= 1) && (u <= getOrder(G))) {
        return G->parent[u];
    } else {
        printf("Graph error: calling getParent() on u not in range.\n");
        exit(EXIT_FAILURE);
    }
}

int getDiscover(Graph G, int u) {
    /* Pre: 1<=u<=n=getOrder(G) */
    if ((1 <= u) && (u <= getOrder(G))) {
        return G->discover[u];
    } else {
        printf("Graph error: calling getDiscover() on u not in range.\n");
        exit(EXIT_FAILURE);
    }
}

int getFinish(Graph G, int u) {
    /* Pre: 1<=u<=n=getOrder(G) */
    if ((1 <= u) && (u <= getOrder(G))) {
        return G->finish[u];
    } else {
        printf("Graph error: calling getFinish() on u not in range.\n");
        exit(EXIT_FAILURE);
    }
}

/*** Manipulation procedures ***/
// place(): like append(), but adds while sorting from least to greatest
void place (Graph G, List list, int i) {
    if (length(list) == 0 || index(list) < 0) {
        append(list, i);
        moveFront(list);
    } else if (i <= get(list)) { // if num <= cursor
        if (get(list) == front(list)) {
            prepend(list, i); // if cursor was at front
            return;
        }
        movePrev(list); 
        if (i < get(list)) {
            place(G, list, i);
        } else {
            insertAfter(list, i);
        }
    } else {
        if (get(list) == back(list)) {
            append(list, i); // if cursor was at back
            return;
        }
        moveNext(list);
        if (i > get(list)) {
            place(G, list, i);
        } else {
            insertBefore(list, i);
        }
    }
    return;
}


// addEdge(): adds an edge from vertex u to v
void addEdge(Graph G, int u, int v) {
    if (G && ((u >= 1) && (u <= getOrder(G))) && ((v >= 1) && (v <= getOrder(G)))) {
        if (G->neighbors[u]) {
            addArc(G, v, u);
            addArc(G, u, v);
            G->size -= 1;
        } else {
            printf("Graph error: calling addEdge() to a vertex that does not exist or not in range.\n");
            exit(EXIT_FAILURE);
        }
    }
    return;
}
// addArc(): adds an arc from u to v
void addArc(Graph G, int u, int v) {
    if (G && ((u >= 1) && (u <= getOrder(G))) && ((v >= 1) && (v <= getOrder(G)))) {
        if (G->neighbors[u] && (find(G->neighbors[u], v) == false)) {
            place(G, G->neighbors[u], v);
            G->size += 1;
        }
    } else {
        printf("Graph error: calling addArc() to a vertex that does not exist or not in range.\n");
        exit(EXIT_FAILURE);
    }
    return;
}


void visit(Graph G, int x, int *time, List S) {
    (*time) += 1;
    G->discover[x] = (*time);
    G->color[x] = 'g';
    // for (moveFront(G->neighbors[x]); index(G->neighbors[x]) >= 0; moveNext(G->neighbors[x])) {
    moveFront(G->neighbors[x]);
    while (index(G->neighbors[x]) >= 0) {
    int cur = get(G->neighbors[x]);
        if (G->color[cur] == 'w') {
            G->parent[cur] = x;
            visit(G, cur, time, S);
        }
        moveNext(G->neighbors[x]);
    }
    G->color[x] = 'b';
    (*time) += 1;
    G->finish[x] = (*time);
    prepend(S, x);
    return;
}

void DFS(Graph G, List S) {
    /* Pre: length(S)==getOrder(G) */
    if (G && (length(S) == getOrder(G))) {
        for (int i = 1; i <= getOrder(G); i += 1) {
            G->color[i] = 'w';
            G->parent[i] = NIL;
        }
        int time = 0;
        moveFront(S);
        while (index(S) >= 0) {
            if (G->color[get(S)] == 'w') {
                visit(G, get(S), &time, S);
            }
            moveNext(S);
        }
        for (int i = 1; i <= getOrder(G); i += 1) {
            deleteBack(S);
        }
    }
    return;
}

/*** Other Functions ***/
// transpose(): returns the transpose graph of G
Graph transpose(Graph G) {
    Graph N = newGraph(G->vertices);
    for (int i = 0; i <= getOrder(G); i += 1) {
        for (moveFront(G->neighbors[i]); index(G->neighbors[i]) >= 0; moveNext(G->neighbors[i])) {
            addArc(N, get(G->neighbors[i]), i);
        }
    }
    return N;
}

Graph copyGraph(Graph G) {
    Graph N  = newGraph(G->vertices);
    for (int i = 1; i <= G->vertices; i += 1) {
        N->neighbors[i] = G->neighbors[i];
        N->parent[i] = G->parent[i];
        N->color[i] = G->color[i];
        N->discover[i] = G->discover[i];
        N->finish[i] = G->finish[i];
    }
    return N;
}

void printGraph(FILE* out , Graph G) {
    if (G) {
        for (int i = 1; i <= G->vertices; i += 1) {
            fprintf(out, "%d: ", i);
            if (length(G->neighbors[i]) > 0) {
                printList(out, G->neighbors[i]);
            } else {
                fprintf(out, "\n");
            }
        }
    }
    return;
}


