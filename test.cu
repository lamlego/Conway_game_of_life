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

// typedef struct {
//     bool *core;
//     bool *top;
//     bool *bot;
// } game_map;
// //generate a game map with side borders but not above or below
// bool* create_world(int m, int n){
//     m = m+2;
//     bool * world;//temperory grid to hold randomly generated world
//     world = new bool[m*n]();
//     srand(time(0));
//     //assigning random values to world except for borders
//     for(int i = 0; i < n ; i++){
//         for(int j = 1; j< m-1; j++){
//             world[(i*m)+j] = rand()%2;
//         }
//     }
//     return world;
// }
//takes a world and split it into 3 parts
// void primal_split(bool **world,bool **top, bool **bot,int m, int n){
//     m = m+2;
//     int count = m;
//     bool *t = *top;
//     bool *w = *world;
//     bool *b = *bot;
//     for(int i = 0; i< m*n; i++){
//         t[count] = w[i];
//         //cout<<w[i];
//         count++;
//     }
//     count = 0;
//     for(int i = m; i< m*n; i++){

//         b[count] = w[i];
//         count++;
//     }
// }

// __global__
// void flip(bool *top, bool *core, bool *bot, int *res,int n){
//     int i = blockIdx.x * blockDim.x + threadIdx.x;
//     if(i<n){
//         printf("hi %d\n",core[i]);
//             res[i] = top[i]+core[i]+bot[i];
            
//     }
    

// }

__global__
void add(int *a,int *b,int *c,int n){
    int i = blockDim.x*blockIdx.x + threadIdx.x;
    c[i]= a[i] +b[i];

}

int main(int argc, char *argv[]){
    int size = 100000* sizeof(int);
    int *a, *b, *c;
    int *d_a, *d_b, *d_c; 
    a = (int *)malloc(size);
    b = (int *)malloc(size);
    c = (int *)malloc(size);   
    for(int i=0;i<100000;i++){
        a[i]=i;
        b[i]=i;
    }
    cout<<a[0]<<"first element in a\n";
    cudaMalloc((void**)&d_a,size);
    cudaMalloc((void**)&d_b,size);
    cudaMalloc((void**)&d_c,size);

    cudaMemcpy(d_a,a,size,cudaMemcpyHostToDevice);
    cudaMemcpy(d_b,b,size,cudaMemcpyHostToDevice);
    cudaDeviceSynchronize();
    cout<<"before calling kernel\n";
    add<<<10,64>>>(d_a,d_b,d_c,100000);
    cout<<"after calling kernel\n";
    cudaMemcpy(c,d_c,size,cudaMemcpyDeviceToHost);
    cout<<"first 100 elements in sum vector c\n";
    for(int i=0;i<100;i++){
        cout<<c[i]<<" ";
    }
 
}