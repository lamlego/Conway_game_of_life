#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <chrono>	 // timing libraries
#include <vector>
#include <algorithm>
using namespace std;

/**takes the number of living cell and the current bit
 * returns an int
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
//creates a matrix of m by n and loading cells with 0|1
vector<bool> create_world(int m, int n)
{
    
    vector<bool> world(m*n,0);
    //world = new bool*[m];
    srand(time(0));
    for(int i = 1; i < m-1 ; i++){
        for(int j = 1; j< n-1; j++){
               world[i*m+j]= rand()%2;
        }
    }
    return world;
}
//takes a 2d int matrix and dimentions and print it
void print_matrix(vector<bool> world,int m, int n){
    for (int i = 0; i < m; i++){
            cout<<"| ";
            for(int j =0; j < n; j++){
                cout<<world[i*m+j]<<" ";
            }
            cout<<"|\n";
        }
}

//return the time it take to run next turn for it number of iterations
auto bench_mark(vector<bool>(*func)(vector<bool>,int,int),vector<bool> world, int m, int n,int it){
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
vector<bool> next_turn(vector<bool> world,int m,int n)
{
    vector<bool> new_world (m*n,0); 
    //new_world = new bool*[m];
    //new_world.push_back(vector<bool>());
    //cout<< "in next turn";
    
    for(int i = 1;i < m-1 ;i++){ 
        for(int j = 1; j < n-1; j++){
                int living = world[(i-1)*m+(j-1)] + world[(i-1)*m+j] + world[(i-1)*m+(j+1)] +
                             world[i*m+(j+1)] + world[i*m+(j-1)] + world[(i+1)*m+(j-1)] + world[(i+1)*m+j] +
                             world[(i+1)*m+(j+1)]  ;

                new_world[i*m+j] = dead_or_alive(living, world[i*m+j]);
        }
    }
    //delete[] world;

    return new_world;
}

int main(int argc, char *argv[]){
    //maybe set it up so these variables can be input from user
    if(argc != 5){
        cout<<"Usage: ./gol [width] [height] [iterations] [number of tests](optional)";
        return 1;
    }
    
    int m = atoi(argv[1]);
    m = m + 2;
    int n = atoi(argv[2]);
    n = n + 2;
    int iterations = atoi(argv[3]);
    int num_tests = atoi(argv[4]);
    
    //Checking data that was entered making sure it is INT
    if((m == 0) || (n == 0) || (iterations == 0) || (num_tests == 0)){
        cout<< "must enter 4 INT values";
        return 0;
    }
    int time = 0;
    vector<vector<bool>> test_cases;
    //cout<< "in main before going to create world";
    //create test cases into a vector
    for (int i = 0; i < num_tests; i++){
        test_cases.push_back(create_world(m,n));
    }
    //cout<<"in main before going to next turn";
    //run each case
    for(vector<vector<bool>>::iterator i = test_cases.begin(); i != test_cases.end(); ++i){
        time +=bench_mark(next_turn,*i,m,n,iterations);
    }
    //print out the average
    cout<< "ran "<< num_tests << " random games of "<< n-2 << " by "<< m-2 << " for "<< iterations<< " iterations, average time is: "<< time/num_tests<<endl;
    /*
    //uncomment this section to print out a iteration
    vector<bool> world= create_world(m,n);
    cout << "our initial matrix\n";
    print_matrix(world,m,n);
    //auto const start_time = std::chrono::steady_clock::now();
    for(int iter = 0; iter < iterations; iter++){// print out each iteration of the matric
        cout<<"\n";
        cout<< iter+1 << "th iteration\n";
        world = next_turn(world,m,n);
        print_matrix(world,m,n);
    }
    */
    //bench_mark(next_turn,world,m,n);
    //auto const end_time = std::chrono::steady_clock::now();
    // cout<< std::chrono::duration_cast<std::chrono::microseconds>( end_time - start_time ).count() << " micro seconds\n";
    //auto const avg_map = csc586::benchmark::benchmark(  next_turn,  world );
}