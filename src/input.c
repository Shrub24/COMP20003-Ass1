#include "input.h"

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include "input.h"

#define DEFAULT_WATCHTOWERS_SIZE 100
#define HEADER_TEMPLATE "%*[^\n]\n"
#define WATCHTOWER_TEMPLATE " %[^,],%[^,],%u,%[^,],%lf,%lf\n "
#define COORDINATE_TEMPLATE "%lf %lf\n"
#define POINT_TEMPLATE "%u %u\n"
#define DYNAMIC_ARR_FACTOR 2


const int WATCHTOWER_LINE_SIZE = 6;
const int COORDINATE_LINE_SIZE = 2;

// We return and do NOT pass in a watchtowers ptr as we want to handle the dynamic memory allocation within this function (abstract it from main) as it will not be mutated in this case
// We don't use a struct for watchtower array here as the insertion logic is only needed/written once
Watchtower **read_watchtowers(FILE *file, uint *size) {
    uint currSize = DEFAULT_WATCHTOWERS_SIZE;

    // declare and initialise dynamic pointer array with default size
    // we don't malloc each individual struct yet as we only need as many as are in the actual file (ie we won't be changing this later), so it is more efficient to malloc them when/if we need them.
    Watchtower **watchtowers = (Watchtower**)malloc(sizeof(Watchtower*) * currSize);

    // skip header
    fscanf(file, HEADER_TEMPLATE);

    // init temp vars
    int i = 0;
    int read = 0;

    while(1) {
        // allocate more memory (multiply size by DYNAMIC_ARR_FACTOR) if we have reached the end of the currently allocated memory
        if ((uint)i == currSize - 1) {
            // realloc a temp array to currSize * DYNAMIC_ARR_FACTOR size.
            Watchtower **temp = (Watchtower**)realloc(watchtowers, sizeof(Watchtower*) * currSize * DYNAMIC_ARR_FACTOR);

            // hand a realloc error, free used memory to minimize leaks
            if (temp == 0) {
                printf("Error allocating more memory to dynamic array");
                free (watchtowers);
                return NULL;
            }

            // if no error we can set the original array to the new temporary one
            watchtowers = temp;
            currSize *= DYNAMIC_ARR_FACTOR;
        }

        // malloc a temporary watchtower to read data into
        Watchtower *temp = (Watchtower*)malloc(sizeof(Watchtower)); 

        // read data into the temporary watchtower and set read to the number of items successfully read (or -1/EOF if EOF)
        read = fscanf(file, WATCHTOWER_TEMPLATE, temp->watchtowerId, temp->postcode, &temp->popServed, temp->contact, &temp->x, &temp->y); 


        // If EOF or an unreadable line (eg. \n), read =/= WATCHTOWER_LINE_SIZE and thus we will have finished reading the CSV, and can now exit the loop and free the temporary watchtower
        if (read != WATCHTOWER_LINE_SIZE) {
            free(temp);
            break;
        }

        // if the read was successful, set i-th watchtower to the read temp watchtower
        watchtowers[i] = temp;

        i++;

    } while(read == WATCHTOWER_LINE_SIZE);
    //condition will ensure \n and EOF will stop the loop.

    // set size pointer to the number of iterations (this counts the iteration that break is reached)
    // use i, not currSize as only i structs have been malloc'd (and need to be free'd)
    *size = i;

    return watchtowers;
}

int read_initial_polygon(FILE *file, DCEL *dcel) {

    Edge *currEdge = NULL;
    Edge *prevEdge = NULL;
    Face *face = (Face*)malloc(sizeof(Face));

    double x, y;    

    while (fscanf(file, COORDINATE_TEMPLATE, &x, &y) == COORDINATE_LINE_SIZE) {
        Vertex *currVertex = (Vertex *)malloc(sizeof(Vertex));
        currVertex->x = x;
        currVertex->y = y;

        if (currEdge == NULL) {
            // first edge in face
            currEdge = (Edge*)malloc(sizeof(Edge));
            face->edge = currEdge;
            currEdge->prev = NULL; 
            currEdge->next = NULL;
            currEdge->end = NULL;
        } else {
            prevEdge = currEdge;
            currEdge = (Edge*)malloc(sizeof(Edge));
            currEdge->prev = prevEdge;
            prevEdge->next = currEdge;
            prevEdge->end = currVertex; 
        }
        
        //initial polygon only has one half-edge per edge
        currEdge->twin = NULL;

        currEdge->start = currVertex;
        currEdge->face = face;

        insertVertex(dcel->vertexList, currVertex);
        insertEdge(dcel->edgeList, currEdge);
    }

    currEdge->next = face->edge;
    face->edge->prev = currEdge->next;
    currEdge->end = face->edge->start; 
    insertFace(dcel->faceList, face);
    return 1;
}


int read_splits(FILE *file, DCEL *dcel) {

    u_int e1, e2;

    while (fscanf(file, POINT_TEMPLATE, &e1, &e2) == COORDINATE_LINE_SIZE) {
        Edge *edge1 = dcel->edgeList->edges[e1];
        Edge *edge2 = dcel->edgeList->edges[e2];

        if (getEdgesToSplit(&edge1, &edge2) == 0) {
            printf("No edges with a shared face to split on!");
            continue;    
        }

        handleSplit(edge1, edge2, dcel); 
    }
    return 1;
}

// Point edge1 and edge2 to the pair of half edges that share a face
int getEdgesToSplit(Edge **edge1, Edge **edge2) {
    if ((*edge1)->face == (*edge2)->face) {
        return 1;
    } else if((*edge1)->twin != NULL) {
        if ((*edge1)->twin->face == (*edge2)->face) {
            (*edge1) = (*edge1)->twin;
            return 1;
        }
        if ((*edge2)->twin != NULL) {
            if ((*edge2)->twin->face == (*edge1)->twin->face) {
                (*edge1) = (*edge1)->twin;   
                (*edge2) = (*edge2)->twin;
                return 1;
            }
        }
    } 
    if ((*edge2)->twin != NULL) {
        if ((*edge2)->twin->face == (*edge1)->face) {
            (*edge2) = (*edge2)->twin;
            return 1;
        }
    }
    return 0;
}

int handleSplit(Edge *edge1, Edge *edge2, DCEL *dcel) {

    Vertex *midpoint1 = getMidpoint(edge1->start, edge1->end);
    Vertex *midpoint2 = getMidpoint(edge2->start, edge2->end);

    Face *newFace = (Face*)malloc(sizeof(Face));
    Edge *newEdge1 = (Edge*)malloc(sizeof(Edge));
    Edge *newEdge2 = (Edge*)malloc(sizeof(Edge));
    Edge *joiningEdge1 = (Edge*)malloc(sizeof(Edge));
    Edge *joiningEdge2 = (Edge*)malloc(sizeof(Edge));

    newFace->edge = joiningEdge2;

    newEdge1->twin = NULL;
    newEdge1->end = edge1->end;
    newEdge1->start = midpoint1;
    newEdge1->prev = joiningEdge2;
    newEdge1->face = newFace;

    newEdge2->twin = NULL;
    newEdge2->end = midpoint2;
    newEdge2->start = edge2->start;
    newEdge2->next = joiningEdge2;
    newEdge2->face = newFace;

    if (edge1->next == edge2) {
        newEdge1->next = newEdge2;
        newEdge2->prev = newEdge1;
    } else {
        newEdge1->next = edge1->next;
        edge1->next->prev = newEdge1;
        newEdge2->prev = edge2->prev;
        edge2->prev->next = newEdge2;
    }

    if (edge1->twin != NULL) {
        newEdge1->twin = (Edge*)malloc(sizeof(Edge));
        newEdge1->twin->twin = newEdge1;
        newEdge1->twin->start = edge1->end;
        newEdge1->twin->end = midpoint1;
        newEdge1->twin->next = edge1->twin;
        newEdge1->twin->prev = edge1->twin->prev;
        newEdge1->twin->face = edge1->twin->face;

        edge1->twin->start = midpoint1;
        edge1->twin->prev = newEdge1->twin;
    }

    if (edge2->twin != NULL) {
        newEdge2->twin = (Edge*)malloc(sizeof(Edge));
        newEdge2->twin->twin = newEdge2;
        newEdge2->twin->start = midpoint2;
        newEdge2->twin->end = edge1->start;
        newEdge2->twin->next = edge2->twin->next;
        newEdge2->twin->prev = edge2->twin;
        newEdge2->twin->face = edge2->twin->face;

        edge2->twin->end = midpoint2;
        edge2->twin->next = newEdge2->twin;
    }

    joiningEdge1->start = midpoint1;
    joiningEdge1->end = midpoint2;
    joiningEdge1->prev = edge1;
    joiningEdge1->next = edge2;
    joiningEdge1->twin = joiningEdge2;
    joiningEdge1->face = edge1->face;

    joiningEdge2->start = midpoint2;
    joiningEdge2->end = midpoint1;
    joiningEdge2->prev = newEdge2;
    joiningEdge2->next = newEdge1;
    joiningEdge2->twin = joiningEdge1;
    joiningEdge2->face = newFace;

    Edge *currEdge = newEdge1->next;

    while(currEdge != newEdge2) {
        currEdge->face = newFace;
        currEdge = currEdge->next;
    }

    edge1->end = midpoint1;
    edge1->next = joiningEdge1;

    edge2->start = midpoint2;
    edge2->prev = joiningEdge1;

    // this order of insertion is IMPORTANT (and is the correct order as per spec of inserting edges)
    insertEdge(dcel->edgeList, joiningEdge1);
    insertEdge(dcel->edgeList, newEdge1);
    insertEdge(dcel->edgeList, newEdge2);

    insertFace(dcel->faceList, newFace);

    insertVertex(dcel->vertexList, midpoint1);
    insertVertex(dcel->vertexList, midpoint2);

    return 1;


}

Vertex *getMidpoint(Vertex *vertex1, Vertex *vertex2) {
    Vertex *midpoint = (Vertex*)malloc(sizeof(Vertex));
    midpoint->x = (vertex1->x + vertex2->x)/2;
    midpoint->y = (vertex1->y + vertex2->y)/2;
    return midpoint;
}