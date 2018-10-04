#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string.h>

double area;
double slice;
int nrOfTrapezes;
std::mutex m;
std::mutex out;
std::mutex slicesLeft;

double fvalue(double x){
    return( 4 / (1 + x*x));
};


void integralthread(int threadNr, int myTrapeze){
    
    out.lock();
    std::cout << "Thread nr " << threadNr << std::endl;
    out.unlock();
    double x1 = slice * myTrapeze;
    double x2 = x1 + slice;
    double f1 = fvalue(x1);
    double f2 = fvalue(x2);
    double rectangleArea = slice * f2;
    
    double triangleArea = (slice * (f1 - f2))/2;
    
    out.lock();
    std::cout << "Thread " << threadNr << " got f1 value " << f1 << std::endl;
    std::cout << "Thread " << threadNr << " got f2 value " << f2 << std::endl;
    std::cout << "Thread " << threadNr << " got area " << rectangleArea+triangleArea << std::endl;
    out.unlock();
    
    m.lock();
    area += rectangleArea + triangleArea;
    m.unlock();
    return;
};

void loop(int threadNr){
    while (nrOfTrapezes > 0){
        slicesLeft.lock();
        double myTrapeze = nrOfTrapezes-1;
        nrOfTrapezes=nrOfTrapezes-1;
        slicesLeft.unlock();
        
        out.lock();
        std::cout << "Thread " << threadNr << " got Trapeze:" << myTrapeze << std::endl;
        out.unlock();
        
        integralthread(threadNr, myTrapeze);
    }
}

void manual(char *program){
    std::cout << "Manual: type " << program << " N (where N=nr of threads > 0 and M=nr of trapezes > 0)" << std::endl;
    exit(1);
}



int main(int argc, char *argv[]){
    if((argc == 1) && (strcmp(argv[1],"-h") == 0)){
    	manual(argv[0]);
    }
    if(argc != 3){
        manual(argv[0]);
    }
       int nrOfThreads;
    try{
        nrOfThreads = std::stoi(argv[1]);
    }
    catch(std::exception){
        manual(argv[0]);
    }
    try{
        nrOfTrapezes = std::stoi(argv[2]);
    }
    catch(std::exception){
        manual(argv[0]);
    }
    if(nrOfThreads < 1 || nrOfTrapezes < 1){
        manual(argv[0]);
    }
    
    slice = 1/(double)nrOfTrapezes; // The size of each trapeze
   
    typedef std::chrono::high_resolution_clock clock; //for measuring time

    std::chrono::time_point<clock> start_time = clock::now();
    
    std::thread *trap = new std::thread[nrOfThreads];
    for (int i=0; i < nrOfThreads; ++i){
        trap[i] = std::thread(loop, i);
       // trap[i].join();
    }
   
    trap[0].join();

    std::chrono::time_point<clock> end_time = clock::now();
    double time = std::chrono::duration<double, std::ratio<1, 1000>>(end_time -start_time).count();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));

    //delete[] trap;
    
    std::cout << "Area is estimated to: " << area << std::endl;
    std::cout << "Computation time: " << time << std::endl;
    return 0;
    
};

