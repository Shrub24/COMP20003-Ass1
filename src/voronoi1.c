#include <stdio.h>
#include "structures.h"
#include "input.h"
#include <stdlib.h>

#define WATCHTOWER_FILE_ARGINDEX = 1;
#define POLYGON_FILE_ARGINDEX = 2;


int main(char **argv)
{

	FILE *watchtowerFile = fopen(argv[1], "r");


    u_int *watchtowerSize = NULL;
    watchtowerSize = malloc(sizeof(u_int));
	//init watchtowers array
    Watchtower** watchtowers = read_watchtowers(watchtowerFile, watchtowerSize);
    fclose(watchtowerFile);

    // free dynamic array of watchtowers
	for (size_t i = 0; i < *watchtowerSize; i++)
	{
		free(watchtowers[i]);
	}
	
    free(watchtowerSize);
	free(watchtowers);


    FILE *polygonFile = fopen(argv[2], "r");
    Face *initialFace = read_initial_polygon(polygonFile);
    fclose(polygonFile);

    // free DCEL
    Edge *currEdge = initialFace->edge;
    Edge *temp = NULL;
    do 
    {
        temp = currEdge;
        currEdge = currEdge->next;
        free(temp->start);
        free(temp);
    } while (currEdge != initialFace->edge);

    free(initialFace);


    

	return 1;
}