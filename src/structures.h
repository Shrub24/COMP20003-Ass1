#ifndef STRUCTURES_H
#define STRUCTURES_H

#define POSTCODE_LENGTH 4
#define WATCHTOWER_ID_LENGTH 11
#define MAX_CONTACT_SIZE 128

typedef struct {
    char watchtowerId[WATCHTOWER_ID_LENGTH + 1];
    char postcode[POSTCODE_LENGTH + 1];
    unsigned int popServed;
    char contact[MAX_CONTACT_SIZE + 1];
    float x;
    float y;

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
    struct edge *edge;
    float x;
    float y;
} Vertex;

typedef struct face {
    struct edge *edge;
} Face;


#endif