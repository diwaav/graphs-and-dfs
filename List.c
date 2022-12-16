/********************************************************************************* 
* Diwa Ashwini Vittala, dashwini
* 2022 Spring CSE101 PA3
* List.c 
* List ADT 
**********************************************************************************/

#include "List.h"

typedef struct NodeObj* Node;

// node structure
typedef struct NodeObj {
    int data;
    Node next;
    Node previous;
} NodeObj;

// list structure
typedef struct ListObj {
    Node front;
    Node back;
    int length;
    Node cursor;
    int index;
} ListObj;

// Constructors-Destructors ---------------------------------------------------
// newNode(): construct a node
// INPUTS: data
// OUTPUTS: a new node
Node newNode(int data) {
    Node N = malloc(sizeof(NodeObj));
    if (N) {
        N->data = data;
        N->previous = N->next = NULL;
        return N;
    } else {
        printf("Node Error: unable to allocate memory in newNode().\n");
        exit(EXIT_FAILURE);
    }
}

// newList(): construct a list
// INPUTS: void
// OUTPUTS: a new list
List newList(void) {
    // Creates and returns a new empty List
    List L = malloc(sizeof(ListObj));
    if (L) {
        L->length = 0;
        L->index = -1;
        L->front = L->back = L->cursor = NULL;
        return L;
    } else {
        printf("Node Error: unable to allocate memory in newNode().\n");
        exit(EXIT_FAILURE);
    }
}

// freeNode(): destruct a node
// INPUTS: a node pointer
// OUTPUTS: void
void freeNode(Node* pN) {
    if (pN && *pN) {
        free(*pN);
        *pN = NULL;
    }
    return;
}

// freeList(): destruct a list
// INPUTS: a list pointer
// OUTPUTS: void
void freeList(List* pL) {
    // Frees all heap memory associated with *pL, and sets
    // *pL to NULL.
    if (pL && (*pL)) {
        clear(*pL);
        free(*pL);
        *pL = NULL;
    }
    return;
}

// Access functions -----------------------------------------------------------
// length(): returns the length of a list
int length(List L) {
    if (!L) {
        printf("List Error: calling length() on NULL List\n");
        exit(EXIT_FAILURE);
    } else {
        return L->length;
    }
}

// index(): returns index of cursor element if defined, -1 otherwise.
int index(List L) {
    if (!L) {
        printf("List Error: calling index() on NULL List.\n");
        exit(EXIT_FAILURE);
    } else if (!L->cursor) {
        return -1;
    } else {
        return L->index;
    }
}

// front(): returns front element of L. Pre: length()>0
int front(List L) {
    if (length(L) <= 0) {
        printf("List Error: calling front() on NULL/empty List.\n");
        exit(EXIT_FAILURE);
    } else {
        return (L->front)->data;
    }
}

// back(): returns back element of L. Pre: length()>0
int back(List L) {
    if (length(L) <= 0) {
        printf("List Error: calling back() on NULL/empty List.\n");
        exit(EXIT_FAILURE);
    } else {
        return (L->back)->data;
    }
}

// get(): returns cursor element of L. Pre: length()>0, index()>=0
int get(List L) {
    if (length(L) <= 0) {
        printf("List Error: calling get() on NULL/empty List.\n");
        exit(EXIT_FAILURE);
    } else if (index(L) < 0) {
        printf("List Error: calling get() on List without cursor index.\n");
        exit(EXIT_FAILURE);
    } else {
        return (L->cursor)->data;
    }
}

// equals(): checks if two lists are equal
bool equals(List A, List B) {
    if (A && B) {
        if (A->length != B->length) {
            return false;
        } else if (A->length == 1) {
            return ((A->front)->data == (B->front)->data);
        } else {
            // look through each element and compare starting at the front
            Node N = A->front;
            Node M = B->front;
            while (N != A->back) {
                if (N->data != M->data) {
                    return false;
                } else {
                    N = N->next;
                    M = M->next;
                }
            }
            return true;
        }
    } else {
        printf("List Error: calling equals() when one of the lists don't exist.\n");
        exit(EXIT_FAILURE);
    }
}

// Manipulation procedures ----------------------------------------------------
// clear(): resets L to its original empty state.
void clear(List L) {
    if (L) {
        if (length(L) == 0) {
            return;
        }
        while(L->front) {
            deleteFront(L);
        }
        L->cursor = L->front = L->back = NULL;
        L->index = -1;
    }
    return;
}

// set(): Overwrites the cursor element’s data with x. Pre: length()>0, index()>=0
void set(List L, int x) {
    if ((length(L) > 0) && (index(L) >= 0)) {
        (L->cursor)->data = x;
    }
    return;
}

// moveFront(): If L is non-empty, sets cursor under the front element, otherwise does nothing.
void moveFront(List L) {
    if (length(L) > 0) {
        L->cursor = L->front;
        L->index = 0;
    }
    return;
} 

// moveBack(): If L is non-empty, sets cursor under the back element, otherwise does nothing.
void moveBack(List L) {
    if (length(L) > 0) {
        L->cursor = L->back;
        L->index = L->length - 1;
    }
    return;
}

// movePrev(): If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing
void movePrev(List L) {
    if (L->cursor && (L->cursor != L->front)) {
        L->cursor = (L->cursor)->previous;
        L->index -= 1;
    } else if (L->cursor && (L->cursor == L->front)) {
        L->index = -1;
        L->cursor = NULL;
    } else {
        return;
    }
}

// moveNext(): If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing
void moveNext(List L) {
    if (L->cursor && (L->cursor != L->back)) {
        L->cursor = (L->cursor)->next;
        L->index += 1;
    } else if (L->cursor && (L->cursor == L->back)) {
        L->index = -1;
        L->cursor = NULL;
    } else {
        return;
    }
}

// prepend(): Insert new element into L. If L is non-empty,
// insertion takes place before front element.
void prepend(List L, int x) {
    if (L->length == 0) {
        Node N = newNode(x);
        N->previous = N->next = NULL;
        L->length += 1;
        L->front = L->back = N;
        L->index += 1;
    } else if (L->length > 0) {
        Node N = newNode(x);
        N->previous = NULL;
        N->next = L->front;
        (L->front)->previous = N;
        L->front = N;
        L->length += 1;
        L->index += 1;
    } else {
        printf("List Error: Something went wrong in prepend().\n");
        exit(EXIT_FAILURE);
    }
    return;
}


// append(): Insert new element into L. If L is non-empty,
// insertion takes place after back element.
void append(List L, int x) {
    if (L->length == 0) {
        Node N = newNode(x);
        N->previous = N->next = NULL;
        L->length += 1;
        L->front = L->back = N;
    } else if (L->length > 0) {
        Node N = newNode(x);
        N->next = NULL;
        N->previous = L->back;
        (L->back)->next = N;
        L->back = N;
        L->length += 1;
    } else {
        printf("List Error: Something went wrong in append().\n");
        exit(EXIT_FAILURE);
    }
    return;
}


// insertBefore(): Insert new element before cursor.
// Pre: length()>0, index()>=0
void insertBefore(List L, int x) {
    if (L->length <= 0 || L->index < 0) {
        printf("List Error: calling insertBefore() on empty list, or a list without cursor.\n");
        exit(EXIT_FAILURE);
    } else if (L->cursor == L->front) {
        prepend(L, x);
    } else {
        Node N = newNode(x);
        N->previous = (L->cursor)->previous;
        ((L->cursor)->previous)->next = N;
        N->next = L->cursor;
        (L->cursor)->previous = N;
        if (L->cursor == L->front) {
            L->front = N;
        }
        L->length += 1;
        L->index += 1;
    }
    return;
}

// insertAfter(): Insert new element after cursor.
// Pre: length()>0, index()>=0
void insertAfter(List L, int x) {
    if (L->length <= 0 || L->index < 0) {
        printf("List Error: calling insertAfter() on empty list, or a list without cursor.\n");
        exit(EXIT_FAILURE);
    } else if (L->cursor == L->back) {
        append(L, x);
    } else {
        Node N = newNode(x);
        N->next = (L->cursor)->next;
        ((L->cursor)->next)->previous = N;
        N->previous = L->cursor;
        (L->cursor)->next = N;
        if (L->cursor == L->back) {
            L->back = N;
        }
        L->length += 1;
    }
    return;
}

// deleteFront(): Delete the front element. Pre: length()>0
void deleteFront(List L) {
    if (L->length <= 0) {
        printf("List Error: calling deleteFront() on empty list.\n");
        exit(EXIT_FAILURE);
    } else if ((L->front)->next) {
        Node N = (L->front)->next;
        L->index -= 1;
        if (L->cursor == L->front) {
            L->cursor = NULL;
            L->index = -1;
        }
        freeNode(&(L->front));
        L->front = N;
        (L->front)->previous = NULL;
        L->length -= 1;
    } else {
        freeNode(&(L->front));
        L->front = L->back = NULL;
        L->length -= 1;
        L->cursor = NULL;
        L->index = -1;
    }
    return;
}

// deleteBack(): Delete the back element. Pre: length()>0
void deleteBack(List L) {
    if (L->length <= 0) {
        printf("List Error: calling deleteBack() on empty list.\n");
        exit(EXIT_FAILURE);
    } else if ((L->back)->previous) {
        Node N = (L->back)->previous;
        if (L->cursor == L->back) {
            L->cursor = NULL;
            L->index = -1;
        }
        freeNode(&(L->back));
        L->back = N;
        (L->back)->next = NULL;
        L->length -= 1;
    } else {
        freeNode(&(L->back));
        L->front = L->back = NULL;
        L->length -= 1;
        L->cursor = NULL;
        L->index = -1;
    }
    return;
}

// delete(): Delete cursor element, making cursor undefined.
// Pre: length()>0, index()>=0
void delete(List L) {
    if (L->length <= 0 || L->index < 0) {
        printf("List Error: calling delete() on an empty list or list without index.\n");
        exit(EXIT_FAILURE);
    } else if (L->length == 1) { 
        // the only element is the cursor element
        freeNode(&(L->cursor));
        L->cursor = L->front = L->back = NULL;
        L->index = -1;
        L->length -= 1;
    } else if (L->cursor == L->front) {
        // cursor is first element
        deleteFront(L);
    } else if (L->cursor == L->back) {
        // cursor is last element
        deleteBack(L);
    } else {
        Node before = (L->cursor)->previous;
        Node after = (L->cursor)->next;
        before->next = after;
        after->previous = before;
        freeNode(&(L->cursor));
        L->cursor = NULL;
        L->index = -1;
        L->length -= 1;
    }
    return;
}

bool find(List L, int x) {
  Node N = NULL;
  for (N = L->front; N != NULL; N = N->next) {
        if (N->data == x) {
          return true;
        }
  }
  return false;
}

// Other operations -----------------------------------------------------------
// printList(): Prints to the file pointed to by out, a
// string representation of L consisting
// of a space separated sequence of integers,
// with front on left.
void printList(FILE* out, List L) {
    Node N = NULL;
    if (!L) {
        printf("List Error: calling printList() on NULL List\n");
        exit(EXIT_FAILURE);
    }
    for (N = L->front; N != NULL; N = N->next) {
        fprintf(out, "%d ", N->data);
    }
    fprintf(out, "\n");
    return;
}

// copyList(): Returns a new List representing the same integer
// sequence as L. The cursor in the new list is undefined,
// regardless of the state of the cursor in L. The state
// of L is unchanged.
List copyList(List L) {
    if (L->length == 0) {
        return newList();
    } else if (L->length == 1) {
        List A = newList();
        prepend(A, (L->front)->data);
        return A;
    } else {
        List A = newList();
        append(A, (L->front)->data);
        Node N = (L->front)->next;
        Node M = A->front;
        while (N != L->back) {
            append(A, N->data);
            N = N->next;
            M = M->next;
        }
        if (N == L->back) {
            append(A, N->data);
        }
        return A;
    } 
}


// printtofile(): my own function created to print a list to
// an output file. This is the same as printList() but prints the 
// actual strings from the array instead of just the index
void printtofile(FILE* out, List L, char **array) {
    Node N = NULL;
    for (N = L->front; N != NULL; N = N->next) {
        fprintf(out, "%s\n", array[N->data]);
    } 
    return;
}



