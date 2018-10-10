#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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

#define INFINITY 1000; 
 

static int ** allocate_array(int N) {
	int ** array;
	int i, j, randomNr;
	array = (int**) malloc(N * sizeof(int*));
	for (i = 0; i < N ; i++){
		array[i] = (int*)  malloc(N * sizeof(int));
	}
	for (i = 0; i < N ; i++){
		for (j = 0; j < N ; j++){
			if(i == j){
				array[i][j] = 0; 
			}
			else{
				randomNr = rand() % 20; //random value
				if(randomNr > 10){
					array[i][j] = INFINITY; 
				}
				else{
					array[i][j] = randomNr;
				}

				//printf("array: i =%i, j = %i, with random number %i \n", i, j, randomNr);
			}
		}
	}
	return array;
}

static void free_array(int ** array, int N) {
	int i;
	for (i = 0 ; i < N ; i++){
		free(array[i]);
	}
	free(array);
}

static void compute_tile(int **previous, int **current, int starti, int startj, int tilesize){
	int t,i,j,end,oldValue,newValue;
	end = starti + tilesize;
	for (t = 1 ; t < end ; t++) {
		for (i = starti ; i < end ; i++){
			for (j = startj ; j < end; j++) {
				oldValue = previous[i][j];
				newValue = previous[i][t] + previous[t][j];
				if(oldValue > newValue){
					current[i][j] = newValue;					
				}	
			
			}
		}

	}
}

 
int main (int argc, char * argv[]) {
	int N;						//size of array 			
	int ** current, **previous;				 	//array
	int k, i, j, nrOfThreads, tilesize;	//helper variables

	double time;
	struct timeval ts,tf; 		//for timing
	
	/*Read input arguments*/
	if (argc != 3) {
		fprintf(stderr, "Usage: ./exec ArraySize NrOfThreads\n");
		exit(-1);
	}
	else {
		N = atoi(argv[1]);
		nrOfThreads = atoi(argv[2]);
	}

	previous = allocate_array(N); //allocate array
	current = allocate_array(N); //allocate array

	gettimeofday(&ts, NULL);
	
	int nrOfTiles = (N/32)*(N/32);
	
	tilesize = 32;  
	
	// parallelize	
	for(k=0; k <nrOfTiles; k++){
		//dela upp i tre olika tiles: kth tile, E,S,W,N-tiles & the rest
		//
		//vi skickar in top left cell in each tile
		//top left = (N/tilesize)*k
		//(stega i och j i tilesize steg)
		//för alla tiles där i eller j == (k*tilesize): ESWN-tile
		//

		compute_tile(current, previous, i*tilesize, j*tilesize, tilesize);
		previous = current; //both current 

	}


	gettimeofday(&tf, NULL);
	time = (tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;

	free_array(current, N);	
	free_array(previous, N);	
	
	printf("Floyd, array size %i, time %lf\n", N, time); 
	return 0; 

}

 

