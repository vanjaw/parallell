#include <iostream>
#include <math.h>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <mutex>
#include <omp.h>
#include <chrono>

long MAX;
long sqrtMAX;
long ChunkPerThread;
std::vector<long> numbers;
std::vector<long> seeds;


void calculateSeeds(){
    for (long i=1; i<sqrtMAX; i++){
        long k = seeds[i];
        if (k!=0){
            long startValue = (k*k)-1;
            for (long x = startValue; x<sqrtMAX; x++){
                if ((seeds[x] % k) == 0){
                    seeds[x]=0;
                };
            }
        }
    }
}


void manual(char *program){
    std::cout << "Manual: type " << program << " N (nr of threads where N > 0) M (max value where M >0)" << std::endl;
    exit(1);
}

int main(int argc, char *argv[]){ // argc[2] = Max, argc[1] = nrOfThreads
   
    int nrOfThreads;
    if(argc != 3){
        manual(argv[0]);
    }
    try{
        nrOfThreads = std::stoi(argv[1]);
    }
    catch(std::exception){
        manual(argv[0]);
    }
    try{
        MAX = std::stoi(argv[2]);
    }
    catch(std::exception){
        manual(argv[0]);
    }
    if(nrOfThreads < 1 || MAX < 1){
        manual(argv[0]);
    }
    
    sqrtMAX = sqrt(MAX);
    long chunks = MAX - sqrtMAX;
    ChunkPerThread = (chunks+1)/(long)nrOfThreads;
    
    if (ChunkPerThread < 1){
        ChunkPerThread = 1;
        nrOfThreads = MAX-sqrtMAX;
    };
    
    // Fill vector 1:MAX
    for (long i=0; i<MAX; i++){
        numbers.push_back(i+1);
    }
    
    for (long i=0; i<sqrtMAX; i++){
        seeds.push_back(i+1);
    }
 
    calculateSeeds(); //Get seeds
    for (long a = 0; a < sqrtMAX; a++){
        numbers[a] = seeds[a];
    }


    typedef std::chrono::high_resolution_clock clock;
    std::chrono::time_point<clock> start_time = clock::now();
	    
    omp_set_num_threads(nrOfThreads);
    #pragma omp parallel
    {
    int ID = omp_get_thread_num();
    
    //std::cout << "ID: " << ID << std::endl;

    long start = sqrtMAX + (ID*ChunkPerThread);
    long end = start + ChunkPerThread;

    if(ID == nrOfThreads-1){
    	end = MAX;
    }


    for (long i=1; i<sqrtMAX; i++){
        long k = seeds[i];
        
        if (k!=0){
            for (long j = start; j<end; j++){
                if ((numbers[j] % k) == 0){
                    numbers[j]=0;
		}
            }
        }
    }
    } // end of parallel part

   std::chrono::time_point<clock> end_time = clock::now();

   double time = std::chrono::duration<double, std::ratio<1, 1000>>(end_time - start_time).count();


    //PRINT OUT PRIMES
    /*
    std::cout << "Primes are: "<< std::endl;
    for (long i=1; i<MAX; i++){
        if (numbers[i] != 0){
            std::cout << numbers[i] << ", " << std::endl;
        }
    }*/
    std::cout << "Time: " << time << std::endl;
    return 0;
    
};

