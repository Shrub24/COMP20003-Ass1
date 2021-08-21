#ifndef HEADERFILE_H
#define HEADERFILE_H

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include "input.h"


Watchtower **read_watchtowers(FILE *file, uint *size);

int read_initial_polygon(FILE *file, DCEL *dcel);

int read_splits(FILE *file, DCEL *dcel);

int getEdgesToSplit(Edge **edge1, Edge **edge2);

Vertex *getMidpoint(Vertex *vertex1, Vertex *vertex2);

int handleSplit(Edge *edge1, Edge *edge2, DCEL *dcel);


#endif