#include "input.h"

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include "input.h"

#define DEFAULT_WATCHTOWERS_SIZE 100
#define DEFAULT_EDGES_SIZE
#define HEADER_TEMPLATE "%*[^\n]\n"
#define WATCHTOWER_TEMPLATE " %[^,],%[^,],%u,%[^,],%f,%f\n "
#define COORDINATE_TEMPLATE "%f %f\n"
#define DYNAMIC_ARR_FACTOR 2;


const int WATCHTOWER_LINE_SIZE = 6;
const int COORDINATE_LINE_SIZE = 2;

// We return and do NOT pass in a watchtowers ptr as we want to handle the dynamic memory allocation within this function (abstract it from main) as it will not be mutated in this case
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

Face *read_initial_polygon(FILE *file, Face *face, int *size) {
    Edge **edges = malloc(sizeof(Edge*) * DEFAULT_EDGES_SIZE);

    Edge *currEdge = NULL;
    Edge *prevEdge = NULL;

    float x, y;
    u_int edgeIndex;
    

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
    }

    currEdge->next = face->edge;
    face->edge->prev = currEdge->next;
    currEdge->end = face->edge->start; 

    return face;
}


