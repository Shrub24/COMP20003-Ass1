#include <stdio.h>
#include "structures.h"
#include "input.h"
#include <stdlib.h>

#define DEFAULT_SIZE 100



int main(int argc, char **argv)
{

	FILE *watchtowerFile = fopen(argv[0], "r");


    u_int *watchtowerSize = NULL;
    watchtowerSize = malloc(sizeof(u_int));
	//init watchtowers array
    Watchtower** watchtowers = read_watchtowers(watchtowerFile, watchtowerSize);
	for (size_t i = 0; i < *watchtowerSize; i++)
	{
		free(watchtowers[i]);
	}
	
    free(watchtowerSize);
	free(watchtowers);


    // FILE *polygonFile = fopen(argv[1], "r");

	return 1;
}