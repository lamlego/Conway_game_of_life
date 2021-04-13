#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h> 
using namespace std;

void print_matrix(int** world,int m, int n){
    for (int i = 0; i < m; i++){
            cout<<"| ";
            for(int j =0; j < n; j++){
                cout<<world[i][j]<<" ";
            }
            cout<<"|\n";
        }
}

int ** create_empty_world(int m, int n){
    int **world = new int*[m];
    for(int i = 0; i < m; i++){
        world[i] = new int[n];
        for(int j = 0; j < n; j++){
            world[i][j] = 0;
        }
    }

    return world;
}

//just a normal CPU function here. making a world filled with values of 0 or 1 
int ** create_world(int m, int n){
    int **world = new int[m];
    int value;
    for (int i = 0; i < m; i ++){
        world[i] = new int[n];
        for(int j =  0 ; j < n; j ++){
            //setting up padding
            if(i == 0 || j == 0 || i == m || j == n){
                world[i][j] = 0;
            }else{
                srand(time(NULL));
                //giving the world[m][n] a value between 1 and 0
                value = rand() % 2; 
                world[i][j] = value;
            }
        }
    }
    return world; 
}

int dead_or_alive(int living, int current){
    int newcell = current;
    //std::cout<< current <<" ";
    if((current == 1 && (living == 3 || living == 2)) || (current == 0 && living == 3)){
        newcell = 1;
    }
    else {
       newcell = 0;
    }
    
    return newcell;
}

/*
  this will take the world, the new world, the hight and width of the worlds
  We will then go through every value, sum the value of it's 8 neighbours 
  and give a result of 1 or 0 for each cell
  we will do this for the number of turns that we have specified in the command line arguments. 
*/
__global__
void next_turn(int **world, int **new_world, int m, int n ){
    //getting the index that we are currently in 
    int const index_x = threadIdx.x + blockIdx.x * blockDim.x;
    int const index_y = threadIdx.y + blockIdx.y * blockDim.y;
    if(index_x < m){
        if(index_y < n){
            int living = world[index_x-1][index_y-1] + world[index_x-1][index_y] + world[index_x-1][index_y+1] +
                        world[index_x][index_y+1] + world[index_x+1][index_y-1] + world[index_x+1][index_y] +
                        world[index_x+1][index_y+1] + world[index_x][index_y-1];
            new_world[index_x][index_y] = dead_or_alive(living, world[index_x][index_y]);
        }
    }
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
    int **world = create_world(m + 2, n + 2);
    int **new_world = create_empty_world(m + 2, n + 2);

    print_matrix(world, m, n); 

    //create a matrix of ints for the device
    int **dev_world;
    int **dev_new_world;
    cudaMalloc((void **)&dev_world, size);
    cudaMalloc((void **)&dev_new_world, size);
    if(dev_world == NULL) {std::cerr << "not able to alloc memory on device" << std::endl;}
    if(dev_new_world == NULL) {std::cerr << "not able to alloc memory on device" << std::endl;}

    //copy values from host to device
    cudaMemcpy(dev_world, world, size, cudaMemcpyHostToDevice);
    //cudaMemcpy(dev_new_world, new_world, size, cudaMemcpyHostToDevice);
    //no output yet, don't need to copy it over

    //do next_turn on GPU
    for(int i = 0; i < iterations; i ++){
        next_turn<<< NUM_BLOCKS, NUM_BLOCKS, NUM_THREADS_PER_BLOCK >>>(dev_world, dev_new_world, m, n);
    }

    //copy result back to host
    cudaMemcpy(new_world, dev_new_world, size, cudaMemcpyDeviceToHost);

    print_matrix(new_world, m, n);
    //free space that we created
    cudaFree(dev_new_world);
    cudaFree(dev_world);
}