#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h> 
using namespace std;

int ** create_empty_world(int m, int n){
    int world = new int[m];
    for(int i = 0; i < m; i++){
        world[i] = new int[n];
        for(int j; j < n; j++){
            world[i][j] = 0;
        }
    }

    return world;
}

//just a normal CPU function here. making a world filled with values of 0 or 1 
int ** create_world(int m, int n){
    int world = new int[m];
    int value;
    for (int i = 0; i < m; i ++){
        world[i] = new int[n];
        for(int j =  0 ; j < n; j ++){
            srand(time(NULL));
            //giving the world[m][n] a value between 1 and 0
            value = rand() % 2; 
            world[i][j] = value;
        }
    }
    return world; 
}

__global__
void next_turn(int **world, int **new_world, int m, int n ){
    //getting the index that we are currently in 
    int const index = threadIdx.x + blockIdx.x * blockDim.x;
    
}

#define NUM_BLOCKS 1
#define NUM_THREADS_PER_BLOCK 1;
int main(int argc, char *argv[]){
    //verifying the number of arguments
    if(argc <5 || argc> 6){
        cout<<"Usage: ./gol [width] [height] [iterations] [number of tests]";
        return 1;
    }

    //gathering data from the arguments
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    int num_tests = atoi(argv[4]);

    int size = sizeof(int) * m * n; 

    //verifying the arguments are able to be converted to int values
    if((m == 0) || (n == 0) || (iterations == 0) || (num_tests == 0)){
        cout<< "must enter INT values";
        return 0;
    }

    //create a world and a new world
    int **world = create_world(m, n);
    int **new_world = create_empty_world(m, n);

    //create a matrix of ints for the device
    int **dev_world;
    int **dev_new_world;
    cudaMalloc((void **)&dev_world, size);
    cudaMalloc((void **)&dev_new_world, size);

    //copy values from host to device
    cudaMemcpy(dev_world, world, size, cudaMemcpyHostToDevice);
    cudaMemcpy(dev_new_world, new_world, size, cudaMemcpyHostToDevice);

    //do next_turn on GPU

    //free space that we created
    cudaFree(dev_new_world);
    cudaFree(dev_world);
}