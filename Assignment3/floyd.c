#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
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



static void work(int ** array, int starti, int startj, int N){
        int k,i,j,end,oldValue,newValue;
        end = 1; 
        for (k = 1 ; k < N ; k++) {
                for (i = starti ; i < end ; i++){
                        for (j = startj ; j < end; j++) {
                                oldValue = array[i][j];
                                newValue = array[i][k] + array[k][j];
                                if(oldValue > newValue){
                                        array[i][j] = newValue;                                 
                                }       
                        
                        }
                }

        }
}

int main (int argc, char * argv[]) {
	int N;				//size of array 			
	int ** array;		 	//array
	int k, i, j, nrOfThreads, oldValue,newValue;	//helper variables

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

	array = allocate_array(N); //allocate array
	
	gettimeofday(&ts, NULL);

	
	//#pragma omp parallel for private(k,i,j) num_threads(nrOfThreads)
	for (k = 1 ; k < N ; k++) {
		for (i = 1 ; i < N ; i++){
			for (j = 1 ; j < N ; j++) {
				oldValue = array[i][j];
				newValue = array[i][k] + array[k][j];
				if(oldValue > newValue){
					array[i][j] = newValue;					
				//	printf("old: %i, new: %i \n", oldValue,newValue);	
				}	
			
			}
		}
	
	} //end of parallel part
	//#pragma omp barrier

	gettimeofday(&tf, NULL);
	time = (tf.tv_sec-ts.tv_sec)+(tf.tv_usec-ts.tv_usec)*0.000001;

	free_array(array, N);	
	
	printf("Floyd, array size %i, time %lf\n", N, time); 
	return 0; 

}

 

