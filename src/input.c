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
                return;
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

// Face *read_initial_polygon(FILE *file) {
//     Face *face;
//     face = (Face*)malloc(sizeof(Face));

//     int i = -1;
//     uint currSize = DEFAULT_WATCHTOWER_SIZE;
//     int read = 0;
//     Edge *currEdge;

//     do {
//         Vertex *curr = (Vertex *)malloc(sizeof(Vertex));
//         int read = fscanf(file, "%f %f", curr->x, curr->y);
//         if (currEdge == NULL) {
//             face->edge = (Edge*)malloc(sizeof(Edge));
//             currEdge = &face->edge;
//             currEdge->prev = NULL; 
//             currEdge->next = NULL;
//             currEdge->twin = NULL;
//         } else {
//             currEdge->next = (Edge*)malloc(sizeof(Edge));
//             currEdge->end = 
//         }

//         currEdge->face = face


//         read = fscanf(file, WATCHTOWER_TEMPLATE, watchtowers[i]->watchtowerId, watchtowers[i]->postcode, &watchtowers[i]->popServed, watchtowers[i]->contact, &watchtowers[i]->x, &watchtowers[i]->y);
//     } while(read == WATCHTOWER_LINE_SIZE);
    
// }