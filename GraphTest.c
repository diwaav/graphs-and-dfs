/********************************************************************************* 
* Diwa Ashwini Vittala, dashwini
* 2022 Spring CSE101 PA2
* GraphTest.c
* A program to test Graph.c
**********************************************************************************/

#include "Graph.h"
#include "List.h"

#include <stdio.h>
#include <stdlib.h>


int main(void) {
    Graph A = newGraph(11);
    List L = newList();

    addArc(A, 1, 2);
    addArc(A, 2, 5);
    addArc(A, 3, 2);
    addArc(A, 3, 5);
    addArc(A, 4, 1);
    addArc(A, 5, 4);
    addArc(A, 6, 3);
    addArc(A, 6, 5);
    addArc(A, 6, 9);
    addArc(A, 6, 10);
    addArc(A, 7, 3);
    addArc(A, 7, 6);
    addArc(A, 8, 4);
    addArc(A, 9, 4);
    addArc(A, 9, 5);
    addArc(A, 9, 8);
    addArc(A, 10, 9);
    addArc(A, 10, 11);
    addArc(A, 11, 7);

    printf("The Graph has %d vertices, and size is %d\n", getOrder(A), getSize(A));

    printf("ORIGINAL:\n");
    DFS(A, L);
    printGraph(stdout, A);

    printf("\nTRANSPOSE:\n");
    Graph N = transpose(A);
    printGraph(stdout, N);

    freeList(&L);
    freeGraph(&N);
    freeGraph(&A);
    return 0;
}

/* output should look like this:
The Graph has 11 vertices, and size is 19
ORIGINAL:
1: 2 
2: 5 
3: 2 5 
4: 1 
5: 4 
6: 3 5 9 10 
7: 3 6 
8: 4 
9: 4 5 8 
10: 9 11 
11: 7 

TRANSPOSE:
1: 4 
2: 1 3 
3: 6 7 
4: 5 8 9 
5: 2 3 6 9 
6: 7 
7: 11 
8: 9 
9: 6 10 
10: 6 
11: 10 
*/
