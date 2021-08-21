#ifndef HEADERFILE_H
#define HEADERFILE_H

#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include "input.h"


Watchtower **read_watchtowers(FILE *file, uint *size);

Edge **read_initial_polygon(FILE *file, Face *face, int *size);


#endif