#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

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
    std::cout << "Manual: type " << program << " N (where N > 0)" << std::endl;
    exit(1);
}

int main(int argc, char *argv[]){
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
    
    std::thread *trap = new std::thread[nrOfThreads];
    for (int i=0; i < nrOfThreads; ++i){
        trap[i] = std::thread(loop, i);
        trap[i].join();
    }
    
//    trap[0].join();

    delete[] trap;
    
    std::cout << "Area is estimated to: " << area << std::endl;
    return 0;
    
};

