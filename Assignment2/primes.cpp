//
//  main.cpp
//  Assignment12
//
//  Created by Vanjas on 2018-09-27.
//  Copyright © 2018 Vanjas. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <thread>

int k;
int nrOfThreads;
int MAX;
int array[] = {};


void algoritm(int k){
    int startValue = (k*k)-1;
    for (int i = startValue; i<(MAX); i++){
        if ((array[i] % k) == 0){
            array[i]=0;
        };
    };
}

void checkForPrimes(int start, int end, int threadNr){
    // CHECK FOR PRIMES
    std::cout << "Thread " << threadNr << " is working with start: " << start << " and end: " << end  << std::endl;
    for (int k = start; k<=end; k++){
        if (k != 0){
            algoritm(k);
        }
    }
}

void manual(char *program){
    std::cout << "Manual: type " << program << " N (nr of threads where N > 0) M (max value where M >0)" << std::endl;
    exit(1);
}

int main(int argc, char *argv[]){ // argc[2] = Max, argc[1] = nrOfThreads
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
    
    // Fill array 1:MAX
    for (int i=0; i<MAX; i++){
        array [i] = i+1;
    }
    
    int sqrtMAX = sqrt(MAX);
    int chunks = MAX - sqrtMAX;
    int ChunkPerThread = (chunks+1)/(int)nrOfThreads;
   
    if (ChunkPerThread < 1){
        ChunkPerThread = 1;
        nrOfThreads = MAX-sqrtMAX;
    };
    
    std::cout << "SCRT: " << sqrtMAX << std::endl;
    std::cout << "Chunks: " << chunks << std::endl;
    std::cout << "Chunk per thread: " << ChunkPerThread << std::endl;
    std::cout << "Threads: " << nrOfThreads << std::endl;
    
    std::thread *th = new std::thread[nrOfThreads];
    for (int i=0; i < nrOfThreads; ++i){
        
        int startValue = sqrtMAX+(i*ChunkPerThread)+1;
        int endValue = startValue + ChunkPerThread-1;
        
        if (i == 0) {
            startValue = 2;
            endValue = sqrtMAX+ChunkPerThread;
        }
        else if (i == (nrOfThreads-1)) {
            endValue = MAX;
        }
        th[i] = std::thread(checkForPrimes, startValue, endValue, i);
        th[i].join();
    }
    
    // PRINT OUT PRIMES
    std::cout << "Primes are: "<< std::endl;
    for (int i=2; i<MAX; i++){
        if (array[i] != 0){
        std::cout << array[i] << ", " << std::endl;
        }
    }
    
    delete[] th;
    return 0;
    
};



//1. Create a list of natural numbers: 1, 2, 3, . . . , Max.
//2. Set k to 2, the first unmarked number in the list.
//3. repeat
//Mark all multiples of k between k2 and Max.
//Find the smallest number greater than k that is still unmarked. Set k to this new value.
//until k2 is greater than Max.
//4. The unmarked numbers are all prime.

//To parallelize this algorithm,
//1. First sequentially compute primes up to √Max.
//2. Given p cores, build p chunks of roughly equal length covering the range from √Max + 1 to Max,
//and allocate a thread for each chunk.
//3. Each thread uses the sequentially computed “seeds” to mark the numbers in its chunk.
//4. The master waits for all threads to finish and collects the unmarked numbers.
