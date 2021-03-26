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
<<<<<<< HEAD
    new_world= new bool[m*n]();
        #pragma omp parallel for
        for(int i = 1;i < m-1 ;i++){ 
            for(int j = 0; j < n; j++){
=======
    new_world= new bool[m*n];
    //uncomment for coarse grained parallelism
    #pragma omp parallel for
    for(int i = 0;i < m ;i++){ 
        //uncomment for fine grained parallelism
        //#pragma omp parallel for
        for(int j = 0; j < n; j++){
            if(i==0||i==m-1||j==0||j==n-1){
                new_world[i*m+j] =0;
            }else{
>>>>>>> 8451cb1a96aa59677ed324abedf432c2ca4dfc01
                int living = world[(i-1)*m+(j-1)] + world[(i-1)*m+j] + world[(i-1)*m+(j+1)] +
                            world[i*m+(j+1)] + world[i*m+(j-1)] + 
                            world[(i+1)*m+(j-1)] + world[(i+1)*m+j] + world[(i+1)*m+(j+1)];
                new_world[i*m+j] = dead_or_alive(living, world[i*m+j]);
            }
        }
    }
    //free(world); this is slower
    delete[] world;
    return new_world;
}

int main(int argc, char *argv[]){
    if(argc <5 || argc> 6){
        cout<<"Usage: ./gol [width] [height] [iterations] [number of tests][number of threads](if not decleared use all)]";
        return 1;
    }
    //set size larger so we can add border of 0 surrounding the map
    int m = atoi(argv[1]);
    m = m + 2;
    int n = atoi(argv[2]);
    n = n + 2;
    int iterations = atoi(argv[3]);
    int num_tests = atoi(argv[4]);
    if(argc==6){
        omp_set_num_threads(atoi(argv[5]));
    }
    //Checking data that was entered making sure it is INT
    if((m == 0) || (n == 0) || (iterations == 0) || (num_tests == 0)){
        cout<< "must enter INT values";
        return 0;
    }
    int time = 0;
    vector<bool*> test_cases;
    //create test cases into a vector
    for (int i = 0; i < num_tests; i++){
        test_cases.push_back(create_world(m,n));
    }
    //run each case
    for(vector<bool*>::iterator i = test_cases.begin(); i != test_cases.end(); ++i){
        time +=bench_mark(next_turn,*i,m,n,iterations);
    }
    //print out the average
    cout<< "ran "<< num_tests << " random games of "<< n-2 << " by "<< m-2 << " for "<< iterations<< " iterations, average time is: "<< time/num_tests<<" us"<<endl;
    /*
    //uncomment this section to print out a iteration
    bool* world= create_world(m,n);
    for(int i=0;i<m*n;i++){
        world[i] =0;
    }
    //this creates stable pattern, turns into a 6x6 cross without center in 3rd turn
    world[(m/2)*m+(n/2)-2] =1;
    world[(m/2)*m+(n/2) -1] =1;
    world[(m/2)*m+(n/2)+1] =1;
    world[(m/2)*m+(n/2)+2] =1;
    world[(m/2-1)*m+(n/2)] =1;
    world[(m/2-2)*m+(n/2)] =1;
    world[(m/2-2)*m+(n/2)-2] =1;
    world[(m/2-2)*m+(n/2)+2] =1;
    world[(m/2+1)*m+(n/2)] =1;
    world[(m/2+2)*m+(n/2)] =1;
    world[(m/2+2)*m+(n/2-2)] =1;
    world[(m/2+2)*m+(n/2+2)] =1;
    cout << "our initial matrix\n";
    print_matrix(world,m,n);
    for(int iter = 0; iter < iterations; iter++){// print out each iteration of the matric
        cout<<"\n";
        cout<< iter+1 << "th iteration\n";
        world = next_turn(world,m,n);
        print_matrix(world,m,n);
    }
    */
}