#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdlib.h>

#define POSTCODE_LENGTH 4
#define WATCHTOWER_ID_LENGTH 11
#define MAX_CONTACT_SIZE 128

typedef struct {
    char watchtowerId[WATCHTOWER_ID_LENGTH + 1];
    char postcode[POSTCODE_LENGTH + 1];
    unsigned int popServed;
    char contact[MAX_CONTACT_SIZE + 1];
    double x;
    double y;

} Watchtower;

typedef struct edge {
    struct edge *twin;
    struct edge *next;
    struct edge *prev;
    struct face *face;
    struct vertex *start;
    struct vertex *end;
} Edge;

typedef struct vertex {
    double x;
    double y;
} Vertex;

typedef struct face {
    struct edge *edge;
} Face;

typedef struct edgeList {
    Edge **edges;
    size_t count;
    size_t size;
} EdgeList;

typedef struct faceList {
    Face **faces;
    size_t count;
    size_t size;
} FaceList;

typedef struct vertexList {
    Vertex **vertices;
    size_t count;
    size_t size;
} VertexList;

typedef struct dcel {
    VertexList *vertexList;
    EdgeList *edgeList;
    FaceList *faceList;
} DCEL;

int insertEdge(EdgeList *edgeList, Edge *edge);

int insertFace(FaceList *faceList, Face *face);

int insertVertex(VertexList *vertexList, Vertex *vertex);

DCEL *initDCEL();

int freeDCEL(DCEL *dcel);


#endif