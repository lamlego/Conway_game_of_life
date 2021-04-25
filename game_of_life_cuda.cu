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
    cout<<"\n";
}

int ** create_empty_world(int m, int n){
    int **world = 0;
    world = new int*[m];
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
    int **world = 0;
    world = new int*[m];
    int value;
    srand(time(0));
    for (int i = 0; i < m; i ++){
        world[i] = new int[n];
        for(int j =  0 ; j < n; j ++){
            //setting up padding
            if(i == 0 || j == 0 || i == m -2 || j == n -2){
                world[i][j] = 0;
            }else{
                //giving the world[m][n] a value between 1 and 0
                value = rand() % 2; 
                world[i][j] = value;
            }
        }
    }
    return world; 
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
    
    if(index_x < m + 1 && index_x != 0){
        for(int index_y = 1; index_y < n - 1 ; index_y++){
            
            int living = world[index_x-1][index_y-1] + world[index_x-1][index_y] + world[index_x-1][index_y+1] +
                        world[index_x][index_y+1] + world[index_x+1][index_y-1] + world[index_x+1][index_y] +
                        world[index_x+1][index_y+1] + world[index_x][index_y-1];

            int current = world[index_x][index_x];
            int new_cell = 0;
            if((current == 1 && (living == 3 || living == 2)) || current == 0 && living == 3){
                new_cell = 1;
            }
            new_world[index_x][index_y] = new_cell;
        }
    }
}

//set up numblocks to be a calculation of how many are needed
int const numblocks = 1;
int const blocksize = 1;
int main(int argc, char *argv[]){
    //verifying the number of arguments
    if(argc <5 || argc> 6){
        cout<<"Usage: ./gol [width] [height] [iterations] [number of tests]";
        return 1;
    }

    //gathering data from the arguments
    int m = atoi(argv[1]);
    m = m + 2;
    int n = atoi(argv[2]);
    n = n + 2;
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

    print_matrix(world, m, n); 

    cout<< "about to copy stuff over";
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

    //set up timing here, no need to take into account how long it takes to copy stuffs over. 

    //do next_turn on GPU
    for(int i = 0; i < iterations; i ++){
        next_turn<<< numblocks, blocksize >>>(dev_world, dev_new_world, m, n);
    }

    //copy result back to host
    cudaMemcpy(new_world, dev_new_world, size, cudaMemcpyDeviceToHost);

    print_matrix(new_world, m, n);
    //free space that we created
    for(int i = 0; i < m ; i++){
        free(world[i]);
        free(new_world[i]);
    }
    free(world);
    free(new_world);
    cudaFree(dev_new_world);
    cudaFree(dev_world);
}