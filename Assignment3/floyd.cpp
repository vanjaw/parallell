#include <stdio>
#include <stdlib.h>
#include <chrono>

/*
 * let V= number of vertices in graph
 * let dist = V x V array of minimum distances
 * for each vertex v:
 * dist[v][v] = 0
 * for each edge (u,v):
 * for k from 1 t oV
 * for i from 1 to V
 * for j from 1 to V:
 * if dist[i][j] > dist[i][k] + dist[k][j]
 * dist[i][j] =  dist[i][k] + dist[k][j]
 *
 */
 
int main (int argc, char * argv[]) {
	int N;	 			
	int ** array;		 	//array
	int k, i, j;			//helper variables

	double time;			//variables for timing
	struct timeval ts,tf;

	/*Read input arguments*/
	if (argc != 2) {
		fprintf(stderr, "Usage: ./exec ArraySize \n");
		exit(-1);
	}
	else {
		N = atoi(argv[1]);
	}

	array = allocate_array(N); //allocate array

	for (k = 1 ; k < N ; k++) {
		for (i = 1 ; i < N ; i++)
			for (j = 1 ; j < N ; j++) {
				oldvalue = array[i][j];
				newValue = array[i][k] + array[k][j];
				if(oldvalue > newvalue){
					array[i][j] = newValue;					
				}	
			
			}
	

	}

	
	
}

 
static int ** allocate_array(int N) {
	int ** array;
	int i, j;
	array = malloc(N * sizeof(int*));
	for (i = 0; i < N ; i++)
		array[i] = malloc(N * sizeof(int));
	for (i = 0; i < N ; i++)
		for (j = 0; j < N ; j++)
			randomnr = rand() % 10; //random value between 0 and 10
			array[i][j] = randomnr;
	return array;
}

static void free_array(int ** array, int N) {
	int i;
	for (i = 0 ; i < N ; i++)
		free(array[i]);
	free(array);
}

