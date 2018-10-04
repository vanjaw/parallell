#include <iostream>
#include <math.h>
//#include <thread>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <mutex>

std::mutex out;
long nrOfThreads;
long MAX;
long sqrtMAX;
std::vector<long> numbers;
std::vector<long> seeds;

struct values {
    long startValue;
    long endValue;
};

void *calculatePrimes(void *v){
    long start = ((struct values *)v)->startValue;
    long end = ((struct values *)v)->endValue;
    
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
    return NULL;
}

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
    long ChunkPerThread = (chunks+1)/(long)nrOfThreads;
    
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
    out.lock();
    std::cout << "SCRT: " << sqrtMAX << std::endl;
    std::cout << "Chunks: " << chunks << std::endl;
    std::cout << "Chunk per thread: " << ChunkPerThread << std::endl;
    std::cout << "Threads: " << nrOfThreads << std::endl;
    out.unlock();
    
    pthread_t threads[nrOfThreads];
    struct values listOfValues[nrOfThreads];
    //std::thread *th = new std::thread[nrOfThreads];
    
    calculateSeeds();
    for (long a = 0; a < sqrtMAX; a++){
        numbers[a] = seeds[a];
    }
    
    for (long i=0; i < nrOfThreads; ++i){
        long startValue = sqrtMAX+(i*ChunkPerThread);
        long endValue = startValue + ChunkPerThread;
        
        /*  if (i == 0) {
         startValue = 2;
         endValue = sqrtMAX;
         }*/
        if (i == (nrOfThreads-1)) {
            endValue = MAX;
        }
        listOfValues[i].startValue = startValue;
        listOfValues[i].endValue = endValue;
        // th[i] = std::thread(checkForPrimes, startValue, endValue, i);
        pthread_create(&threads[i], NULL, &calculatePrimes, &listOfValues[i]);
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
    for (long i=1; i<MAX; i++){
        if (numbers[i] != 0){
            std::cout << numbers[i] << ", " << std::endl;
        }
    }
    out.unlock();
    return 0;
    
};

