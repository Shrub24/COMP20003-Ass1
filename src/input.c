#include "input.h"

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include "input.h"

#define DEFAULT_WATCHTOWER_SIZE 100
#define WATCHTOWER_TEMPLATE " %[^,],%[^,],%u,%[^,],%f,%f\n"

const int WATCHTOWER_LINE_SIZE = 6;
const int DYNAMIC_ARR_FACTOR = 2;

Watchtower **read_watchtowers(FILE *file, uint *size) {    
    // declare and initialise dynamic pointer array with default size
    Watchtower **watchtowers = (Watchtower**)malloc(sizeof(Watchtower*) * DEFAULT_WATCHTOWER_SIZE);

    // skip header
    fscanf(file, "%*[^\n]\n");

    int i = -1;
    uint currSize = DEFAULT_WATCHTOWER_SIZE;
    int read = 0;

    do {
        i++;
        if ((uint)i == currSize - 1) {
            Watchtower **temp = (Watchtower**)realloc(watchtowers, sizeof(Watchtower*) * currSize * DYNAMIC_ARR_FACTOR);
            if (temp == 0) {
                printf("Error allocating more memory to dynamic array");
                return NULL;
            }
            watchtowers = temp;
            currSize *= 2;
        }
        *(watchtowers+i) = (Watchtower*)malloc(sizeof(Watchtower));

        read = fscanf(file, WATCHTOWER_TEMPLATE, watchtowers[i]->watchtowerId, watchtowers[i]->postcode, &watchtowers[i]->popServed, watchtowers[i]->contact, &watchtowers[i]->x, &watchtowers[i]->y);
    } while(read == WATCHTOWER_LINE_SIZE);
    //condition will ensure \n and EOF will stop the loop.

    *size = i + 1;

    return watchtowers;
}

Face *read_initial_polygon(FILE *file) {
    Face *face;
    face = (Face*)malloc(sizeof(Face));

    Edge *currEdge = NULL;
    Edge *prevEdge = NULL;

    float x, y;
    

    while (fscanf(file, "%f %f\n", &x, &y) == 2) {
        Vertex *currVertex = (Vertex *)malloc(sizeof(Vertex));
        currVertex->x = x;
        currVertex->y = y;
        if (currEdge == NULL) {
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

        currEdge->twin = NULL;
        currEdge->start = currVertex;
        currEdge->face = face;
    }

    currEdge->next = face->edge;
    face->edge->prev = currEdge->next;
    currEdge->end = face->edge->start; 

    return face;
}