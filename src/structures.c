#include "structures.h"
#include <stdlib.h>
#include <stdio.h>
#define DYNAMIC_ARR_FACTOR 2
#define DEFAULT_FACES_SIZE 5;
#define DEFAULT_EDGES_SIZE 10


int insertEdge(EdgeList *edgeList, Edge *edge) {
    // allocate more memory (multiply size by DYNAMIC_ARR_FACTOR) if we have reached the end of the currently allocated memory
    if (edgeList->count == edgeList->size) {
        // realloc a temp array to currSize * DYNAMIC_ARR_FACTOR size.
        Edge **temp = (Edge**)realloc(edgeList->edges, sizeof(Edge*) * edgeList->size * DYNAMIC_ARR_FACTOR);

        // hand a realloc error
        if (temp == 0) {
            printf("Error allocating more memory to dynamic array");
            return -1;
        }

        // if no error we can set the original array ptr to the new temporary one
        edgeList->edges = temp;
        edgeList->size *= DYNAMIC_ARR_FACTOR;
    }

    // add new edge to arr
    edgeList->edges[edgeList->count] = edge;

    // increment edge count
    (edgeList->count)++;

    // return the index of the inserted element
    return edgeList->count - 1;
}


// Unsure how to implement this without copying the above. Can use define macros but not sure if there is a better way?
int insertFace(FaceList *faceList, Face *face) {
    // allocate more memory (multiply size by DYNAMIC_ARR_FACTOR) if we have reached the end of the currently allocated memory
    if (faceList->count == faceList->size) {
        // realloc a temp array to currSize * DYNAMIC_ARR_FACTOR size.
        Face **temp = (Face**)realloc(faceList->faces, sizeof(Face*) * faceList->size * DYNAMIC_ARR_FACTOR);

        // hand a realloc error
        if (temp == 0) {
            printf("Error allocating more memory to dynamic array");
            return -1;
        }

        // if no error we can set the original array ptr to the new temporary one
        faceList->faces = temp;
        faceList->size *= DYNAMIC_ARR_FACTOR;
    }

    // add new face to arr
    faceList->faces[faceList->count] = face;

    // increment face count
    (faceList->count)++;

    // return the index of the inserted element
    return faceList->count - 1;
}


// Unsure how to implement this without copying the above. Can use define macros but not sure if there is a better way?
int insertVertex(VertexList *vertexList, Vertex *vertex) {
    // allocate more memory (multiply size by DYNAMIC_ARR_FACTOR) if we have reached the end of the currently allocated memory
    if (vertexList->count == vertexList->size) {
        // realloc a temp array to currSize * DYNAMIC_ARR_FACTOR size.
        Vertex **temp = (Vertex**)realloc(vertexList->vertices, sizeof(Vertex*) * vertexList->size * DYNAMIC_ARR_FACTOR);

        // hand a realloc error
        if (temp == 0) {
            printf("Error allocating more memory to dynamic array");
            return -1;
        }

        // if no error we can set the original array ptr to the new temporary one
        vertexList->vertices = temp;
        vertexList->size *= DYNAMIC_ARR_FACTOR;
    }

    // add new vertex to arr
    vertexList->vertices[vertexList->count] = vertex;

    // increment vertex count
    (vertexList->count)++;

    // return the index of the inserted element
    return vertexList->count - 1;
}


DCEL *initDCEL() {
    DCEL *dcel = (DCEL*)malloc(sizeof(DCEL));

    dcel->faceList = (FaceList*)malloc(sizeof(FaceList));
    dcel->faceList->size = DEFAULT_FACES_SIZE;
    dcel->faceList->faces = (Face **)malloc(sizeof(Face*) * dcel->faceList->size);
    dcel->faceList->count = 0;

    dcel->edgeList = (EdgeList*)malloc(sizeof(EdgeList));
    dcel->edgeList->size = DEFAULT_EDGES_SIZE;
    dcel->edgeList->edges = (Edge **)malloc(sizeof(Edge*) * dcel->edgeList->size);
    dcel->edgeList->count = 0;


    dcel->vertexList = (VertexList*)malloc(sizeof(VertexList));
    dcel->vertexList->size = DEFAULT_EDGES_SIZE;
    dcel->vertexList->vertices = (Vertex **)malloc(sizeof(VertexList*) * dcel->vertexList->size);
    dcel->vertexList->count = 0;

    return dcel;

}


int freeDCEL(DCEL *dcel) {
    for (size_t i = 0; i < dcel->faceList->count; i++)
    {
        free(dcel->faceList->faces[i]);
    }
    free(dcel->faceList->faces);
    free(dcel->faceList);

    for (size_t i = 0; i < dcel->edgeList->count; i++)
    {
        Edge *twin = dcel->edgeList->edges[i]->twin;
        if (twin != NULL) {
            free(twin);
        }
        free(dcel->edgeList->edges[i]);
    }
    free(dcel->edgeList->edges);
    free(dcel->edgeList);

    for (size_t i = 0; i < dcel->vertexList->count; i++)
    {
        free(dcel->vertexList->vertices[i]);
    }
    free(dcel->vertexList->vertices);
    free(dcel->vertexList);

    free(dcel);

    return 1;
    
}