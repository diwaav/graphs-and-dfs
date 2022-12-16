/********************************************************************************* 
* Diwa Ashwini Vittala, dashwini
* 2022 Spring CSE101 PA3
* FindComponents.c
* Main file for graphing algorithm; finds strongly connected components
**********************************************************************************/

#include "List.h"
#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    // There must be an input and output file
    if (!argv[1]) {
        fprintf(stderr, "Error: Missing input or ouptput files.\n");
        exit(EXIT_FAILURE);
    }
    if (argv[3]) {
        fprintf(stderr, "Error: Too many inputs given!\n");
        exit(EXIT_FAILURE);
    }

    // open input and output file 
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL) {
        fprintf(stderr, "Error: no such input file exists.\n");
        exit(EXIT_FAILURE);
    }
    FILE *outfile = fopen(argv[2], "w");

    // read in total number of vertices 
    int numberOfVertices = 0;
    fscanf(infile, "%d\n", &numberOfVertices);

    Graph G = newGraph(numberOfVertices);

    // read in arcs
    int u, v;
    fscanf(infile, "%d %d\n", &u, &v);
    addArc(G, u, v);
    while (true) {
        fscanf(infile, "%d %d\n", &u, &v);
        if ((u == 0) && (v == 0)) {
          break;
        }
        addArc(G, u, v);
    }

    // print adj list
    fprintf(outfile, "Adjacency list representation of G:\n");
    printGraph(outfile, G);
    fprintf(outfile, "\n");

    // A list that acts as a stack
    List S = newList();
    for (int i = 1; i <= numberOfVertices; i += 1) {
        append(S, i);
    }

    // run dfs
    DFS(G, S);
    Graph T = transpose(G);
    DFS(T, S);
    
    printList(stdout, S);

    // count strongly connected components
    int scc = 0;
    for (moveFront(S); index(S) >= 0; moveNext(S)) {
        if (getParent(T, get(S)) == NIL) {
            // this is a new tree
            scc += 1;
        }
    }
    fprintf(outfile, "G contains %d strongly connected components:\n", scc);

    // print the sccs out
    for (int i = 1; i <= scc; i += 1) {
        fprintf(outfile, "Component %d: ", i);
        moveBack(S);
        while (getParent(T, get(S)) != NIL) {
            movePrev(S);
        }
        while (index(S) >= 0) {
            fprintf(outfile, "%d ", get(S));
            moveNext(S);
        }
        moveBack(S);
        while (getParent(T, get(S)) != NIL) {
            deleteBack(S);
            moveBack(S);
        }
        deleteBack(S);
        fprintf(outfile, "\n");
    }

    // free memory
    freeList(&S);
    freeGraph(&T);
    freeGraph(&G);
    fclose(infile);
    fclose(outfile);
    return 0;
}





