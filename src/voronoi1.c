#include <stdio.h>
#include "structures.h"
#include "input.h"
#include <stdlib.h>

#define WATCHTOWER_FILE_ARGINDEX 1;
#define POLYGON_FILE_ARGINDEX 2;


int main(int argc, char **argv)
{

    DCEL *dcel = initDCEL();

	FILE *watchtowerFile = fopen(argv[1], "r");


    u_int *watchtowerSize = NULL;
    watchtowerSize = malloc(sizeof(u_int));
	//init watchtowers array
    Watchtower** watchtowers = read_watchtowers(watchtowerFile, watchtowerSize);
    fclose(watchtowerFile);

    FILE *polygonFile = fopen(argv[2], "r");


    read_initial_polygon(polygonFile, dcel);
    fclose(polygonFile);

    // printf("%ld", edgeList->count);

    FILE *splitFile = fopen(argv[3], "r");
    read_splits(splitFile, dcel);
    fclose(splitFile);



    // free dynamic array of watchtowers
	for (size_t i = 0; i < *watchtowerSize; i++)
	{
		free(watchtowers[i]);
	}
	
    free(watchtowerSize);
	free(watchtowers);


    // free DCEL
    freeDCEL(dcel);
    

	return 1;
}