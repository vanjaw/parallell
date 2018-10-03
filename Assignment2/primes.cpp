#include <iostream>
#include <math.h>
//#include <thread>
#include <vector>
#include <pthread.h>
#include <unistd.h>


std::mutex out;
long k;
long nrOfThreads;
long MAX;
std::vector<long> numbers;

struct values {
    long startValue;
    long endValue;
    long threadNr;
};

void algoritm(long k, long threadnr){
    long startValue = (k*k)-1;
    for (long i = startValue; i<(MAX); i++){
        if ((numbers[i] % k) == 0){
            
            numbers[i]=0;
        };
    };
}

static void *checkForPrimes(void *v){
    long start = ((struct values *)v)->startValue;
    long end = ((struct values *)v)->endValue;
    long threadNr = ((struct values *)v)->threadNr;
    
    // CHECK FOR PRIMES
    out.lock();
    std::cout << "Thread " << threadNr << " is working with start: " << start << " and end: " << end  << std::endl;
    out.unlock();
    for (long k = start; k<=end; k++){
        if (k != 0){
            algoritm(k, threadNr);
        }
    }
    return NULL;
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
    
    // Fill vector 1:MAX
    for (long i=0; i<MAX; i++){
        numbers.push_back(i+1);
    }
    
    long sqrtMAX = sqrt(MAX);
    long chunks = MAX - sqrtMAX;
    long ChunkPerThread = (chunks+1)/(long)nrOfThreads;
    
    if (ChunkPerThread < 1){
        ChunkPerThread = 1;
        nrOfThreads = MAX-sqrtMAX;
    };
    out.lock();
    std::cout << "SCRT: " << sqrtMAX << std::endl;
    std::cout << "Chunks: " << chunks << std::endl;
    std::cout << "Chunk per thread: " << ChunkPerThread << std::endl;
    std::cout << "Threads: " << nrOfThreads << std::endl;
    out.unlock();
    
    pthread_t threads[nrOfThreads];
    struct values listOfValues[nrOfThreads];
    //std::thread *th = new std::thread[nrOfThreads];
    for (long i=0; i < nrOfThreads; ++i){
        long startValue = (i*ChunkPerThread)+1;
        long endValue = startValue + ChunkPerThread-1;
        
        if (i == 0) {
            startValue = 2;
            endValue = ChunkPerThread;
        }
        else if (i == (nrOfThreads-1)) {
            endValue = MAX;
        }
        listOfValues[i].startValue = startValue;
        listOfValues[i].endValue = endValue;
        listOfValues[i].threadNr = i;
        // th[i] = std::thread(checkForPrimes, startValue, endValue, i);
        pthread_create(&threads[i], NULL, &checkForPrimes, &listOfValues[i]);
    }
    out.lock();
    std::cout << "JOINED" << std::endl;
    out.unlock();
    
    //th[0].join();
    //std::this_thread::sleep_for (std::chrono::seconds(1));
    pthread_join(threads[0], NULL);
    sleep(1);
    
    //PRINT OUT PRIMES
    out.lock();
    std::cout << "Primes are: "<< std::endl;
    for (long i=2; i<MAX; i++){
        if (numbers[i] != 0){
            std::cout << numbers[i] << ", " << std::endl;
        }
    }
    out.unlock();
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
