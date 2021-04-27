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

typedef struct {
    bool *core;
    bool *top;
    bool *bot;
} game_map;
//generate a game map with side borders but not above or below
bool* create_world(int m, int n){
    m = m+2;
    bool * world;//temperory grid to hold randomly generated world
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
//takes a world and split it into 3 parts
void primal_split(bool **world,bool **top, bool **bot,int m, int n){
    m = m+2;
    int count = m;
    bool *t = *top;
    bool *w = *world;
    bool *b = *bot;
    for(int i = 0; i< m*n; i++){
        t[count] = w[i];
        //cout<<w[i];
        count++;
    }
    count = 0;
    for(int i = m; i< m*n; i++){

        b[count] = w[i];
        count++;
    }
}

__global__
void flip(bool *top, bool *core, bool *bot, int *res){
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    printf("hi %d\n",core[i]);
    res[i] = top[i]+core[i]+bot[i];
    

}

int main(int argc, char *argv[]){
    //cout<<sizeof(game_map)<<"\n";
    bool *map = create_world(10,10);
    bool *top,*bot;
    bool *d_top, *d_core, *d_bot;
    int *res, *d_res;
    res = (int *)malloc(120*sizeof(int));
    //top = (bool *)malloc(120*sizeof(bool));
    top = new bool[120]();
    bot = new bool[120]();
    //bot = (bool *)malloc(120*sizeof(bool));
    primal_split(&map,&top,&bot,10,10);

    cudaMalloc((void **)&d_top,120*sizeof(bool));
    cudaMalloc((void **)&d_core,120*sizeof(bool));
    cudaMalloc((void **)&d_bot,120*sizeof(bool));
    cudaMalloc((void **)&d_res,120*sizeof(int));
    
    cudaMemcpy(d_top,top,120*sizeof(bool),cudaMemcpyHostToDevice);
    cudaMemcpy(d_core,map,120*sizeof(bool),cudaMemcpyHostToDevice);
    cudaMemcpy(d_bot,bot,120*sizeof(bool),cudaMemcpyHostToDevice);
    
    flip<<<10,12>>>(d_top,d_core,d_bot,d_res);

    cudaMemcpy(res,&d_res,120*sizeof(int),cudaMemcpyDeviceToHost);
    // for(int i = 0;i<10;i++){
    //     for(int j =0;j<12;j++){
    //         cout<<top[i*12+j];
    //     }
    //     cout<< endl;
    // }
    // cout<<"-------\n";
    // for(int i = 0;i<10;i++){
    //     for(int j =0;j<12;j++){
    //         cout<<map[i*12+j];
    //     }
    //     cout<< endl;
    // }
    // cout<<"-------\n";
    // for(int i = 0;i<10;i++){
    //     for(int j =0;j<12;j++){
    //         cout<<bot[i*12+j];
    //     }
    //     cout<< endl;
    // }
    // cout<<"-------\n";
    //cout<<sizeof(map.core);
    for(int i = 0;i<10;i++){
        for(int j =0;j<12;j++){
            cout<<res[i*12+j];
        }
        cout<< endl;
    }
}