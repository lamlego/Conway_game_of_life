#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <chrono>	 // timing libraries
#include <vector>
#include <algorithm>
#include <omp.h>
using namespace std;
/**
*This is an attempt at Conway's game of life in cuda using 
*simt implentation. Right now it has multiple problems and errors 
*that leads to crash
*/

//this name space contain all the funtions that we use in the implmentation
namespace gpu {    
    // creates a (m+2) * n array that represents a m*n map of the game
    // with 0 borders on the left and right side
    bool* create_world(int m, int n){
            m = m+2;
            bool * world;// grid to hold randomly generated world
            world = new bool[m*n]();
            srand(time(0));
            //assigning random values to world except for borders
            for(int i = 0; i < n ; i++){
                for(int j = 1; j< m-1; j++){
                    world[(i*m)+j] = rand()%2;
                }
            }
            return world;
        }
    /** 
     *splits a game map in to top and bottom, world itself does not change
     *the report explains this better with graphics, we call this before calling
     *the kernel
     */
     void split(bool **world,bool **top, bool **bot,int m, int n){
        m = m+2;
        int count = m;
        bool *t = *top;
        bool *w = *world;
        bool *b = *bot;
        for(int i = 0; i< m*n; i++){
            t[count]  = w[i];
            count++;
        }
        count = 0;
        for(int i = m; i< m*n; i++){
            b[count] = w[i];
            count++;
        }
    }
    /**
     * kernel, it uses the 3 arrays that represents the map
     * to determine the next iteration of the map and put it in result
     * each thread is responsible for 1 cell
     * there was thought to use shared memory for top core and bot but we never got to it
     */
    __global__
        void next_turn(bool *top, bool *core, bool* bot, bool *result,int m, int n){
            int i = blockIdx.x * blockDim.x + threadIdx.x;
            int living = 0;
            bool current = core[i];
            //this is for not going out of bound of the array
            if(i < m*n){
                //this is for not checking the bouding 0s
                if(i%m != 0 &&(i+1)%m!=0){
                    living = top[i-1] + top[i] + top[i+1] +
                             core[i-1]+         core[i+1] +
                             bot[i-1] + bot[i] + bot[i+1];
                    //we checked there is no problem figuring out the 
                    //number of living cells
                    //printf("%d living: %d\n",i,living);

                    //the condition of determining the result
                    if(living == 3 || (current&& living ==2)){
                        result[i] = true;
                    }else{
                        result[i] = false;
                    }
                }
                //we know the results are here
                //printf("%d",result[i]);
            }
        }

    /**
     * the function that handles allcating device memory and calling setting 
     * everything up for the kernel
     * the main probem encountered was not being able to copy the results back
     * from device to host
     */
    auto bench_mark(void(*func)(bool *,bool *,bool *,bool *,int,int),bool *world, int m, int n,int it){
        
            int size = (n*(m+2))*sizeof(bool);
            bool *top, *bot;
            bool *d_top, *d_core, *d_bot, *d_result;

            //allcating device memories
            cudaMalloc((void**)&d_top,size);
            cudaMalloc((void **)&d_core,size);
            cudaMalloc((void **)&d_bot,size);
            cudaMalloc((void **)&d_result,size);
            // an attempt to use unified memory to store results
            // actually able to do so but introduced some other problems
            //cudaMallocManaged((void**)&d_result,size);
            
            //top = (bool *)malloc(size);
            //bot = (bool *)malloc(size);
            //this was used over malloc because we need them to be initialized
            //before sending them to split()
            top = new bool[size]();
            bot = new bool[size]();
            
            //start the timer here so time spend allocating memory would not be counted
            auto const start_time = std::chrono::steady_clock::now();

            //the for loop is to keep repeating the function for the number of iterations
            //it is mostly meaningless since we are unable to get the result back to host
            //a better solution might be to create a global function that hadle this and make
            //so we don't need to have too many back and forth between host and device memory
            for (int i=0; i< it; i++){
                gpu::split(&world,&top,&bot,m,n);

                //copy the world to device memory
                cudaMemcpy(d_top, top, size, cudaMemcpyHostToDevice);
                cudaMemcpy(d_core, world, size, cudaMemcpyHostToDevice);
                cudaMemcpy(d_bot, bot, size, cudaMemcpyHostToDevice);

                //Launching kernel
                gpu::next_turn<<<ceil((m+2)*n/1024),1024>>>(d_top,d_core,d_bot,d_result,m+2,n);
                //we need to wait for the device to finish working before copying the result back otherwise 
                //we will copy the memory before we have done anyhing on the kernel
                cudaDeviceSynchronize();

                //we copy the d_result to world so it udates the world for next iteration
                //this does not work for some reason
                //I tested it on other program and it worked accordingly
                //right now the world does not change at all
                cudaMemcpy(world, &d_result,size,cudaMemcpyDeviceToHost);
                
            }
            auto const end_time = std::chrono::steady_clock::now();
            // enableing these leads to many error
            // not sure why 
            // free(top);free(bot);
            // cudaFree(d_core);cudaFree(d_top);cudaFree(d_bot);cudaFree(d_result);
            return(std::chrono::duration_cast<std::chrono::microseconds>( end_time - start_time ).count());
            //cout<< std::chrono::duration_cast<std::chrono::microseconds>( end_time - start_time ).count() << " micro seconds\n";
            }
        
        
       

   



}
//not used
namespace cpu{
/**takes the number of living cell and the current bit
 * returns an boolean
 */
    bool dead_or_alive(int living, bool current){
        bool newcell = current;
        //std::cout<< current <<" ";
        if((current == 1 && (living == 3 || living == 2)) || (current == 0 && living == 3)){
            newcell = 1;
        }
        else {
        newcell = 0;
        }
        
        return newcell;
    }
    //creates a matrix of m by n and loading cells with 0|1 and surround it with a border of 0
    bool * create_world(int m, int n)
    {
        bool* world;
        //creating the array so that it is loaded with 0s to avoid if statments. 
        world = new bool[m*n]();
        
        srand(time(0));
        for(int i = 0; i < m ; i++){
            //only setting the inside box to randomized values of 1 or 0
            for(int j = 1; j< n-1; j++){
                world[i*m+j] = rand()%2;
            }
        }
        return world;
    }

    //takes a 2d int matrix and dimentions and print it
    void print_matrix(bool* world,int m, int n){
        for (int i = 0; i < m; i++){
                cout<<"| ";
                for(int j =0; j < n; j++){
                    cout<<world[i*m+j]<<" ";
                }
                cout<<"|\n";
            }
    }

    //return the time it take to run next turn for it number of iterations
    auto bench_mark(bool*(*func)(bool*,int,int),bool* world, int m, int n,int it){
    auto const start_time = std::chrono::steady_clock::now();
    for(int i = 0; i < it; i++){


        world = func(world,m,n);
    }
    auto const end_time = std::chrono::steady_clock::now();
    return(std::chrono::duration_cast<std::chrono::microseconds>( end_time - start_time ).count());
    //cout<< std::chrono::duration_cast<std::chrono::microseconds>( end_time - start_time ).count() << " micro seconds\n";
    }

    /**
    * takes a 2d int matrix,and the dimentions
    * goes through each cell
    * determine the next frame of the matrix 
    */
    bool * next_turn(bool* world,int m,int n)
    {
        bool* new_world;
        new_world= new bool[m*n]();
        //#pragma omp parallel for
        for(int i = 1;i < m-1 ;i++){ 
            for(int j = 0; j < n; j++){
                int living = world[(i-1)*m+(j-1)] + world[(i-1)*m+j] + world[(i-1)*m+(j+1)] +
                            world[i*m+(j+1)] + world[i*m+(j-1)] + 
                            world[(i+1)*m+(j-1)] + world[(i+1)*m+j] + world[(i+1)*m+(j+1)];
                new_world[i*m+j] = dead_or_alive(living, world[i*m+j]);
            }
        }
        //free(world); this is slower
        delete[] world;
        return new_world;
    }
}
int main(int argc, char *argv[]){
    if(argc <5 || argc> 6){
        cout<<"Usage: ./gol [width] [height] [iterations] [number of tests][number of threads](if not decleared use all)]";
        return 1;
    }
    //set size larger so we can add border of 0 surrounding the map
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    int num_tests = atoi(argv[4]);
    // if(argc==6){
    //     omp_set_num_threads(atoi(argv[5]));
    // }
    //Checking data that was entered making sure it is INT
    if((m == 0) || (n == 0) || (iterations == 0) || (num_tests == 0)){
        cout<< "must enter INT values";
        return 0;
    }
    int time = 0;
    vector<bool*> test_cases;
    //create test cases into a vector
    for (int i = 0; i < num_tests; i++){
        test_cases.push_back(gpu::create_world(m,n));
    }
    // for(int i = 0;i<n;i++){
    //     for(int j =0;j<m+2;j++){
    //         cout<<test_cases[0][i*(m+2)+j];
    //     }
    // cout<< endl;
    // }
    // cout<<"------------\n";
    //run each case
    for(vector<bool*>::iterator i = test_cases.begin(); i != test_cases.end(); ++i){
        time +=gpu::bench_mark(gpu::next_turn,*i,m,n,iterations);
    }
    //print out the average
    cout<< "ran "<< num_tests << " random games of "<< n << " by "<< m << " for "<< iterations<< " iterations, average time is: "<< time/num_tests<<" us"<<endl;
    return 0;
}