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
//creates a matrix of m by n and loading cells with 0|1
int ** create_world(int m, int n)
{
    
    int** world = 0;
    world = new int*[m];
    srand(time(0));
    for(int i = 0; i < m ; i++){
        world[i] = new int[n];
        for(int j = 0; j< n; j++){
            if(i == 0 || i == m-1 || j == 0 || j == n-1){
                world[i][j] = 0;
            }else{
                world[i][j] = rand()%2;
            }
        }
    }
    return world;
}
//takes a 2d int matrix and dimentions and print it
void print_matrix(int** world,int m, int n){
    for (int i = 0; i < m; i++){
            cout<<"| ";
            for(int j =0; j < n; j++){
                cout<<world[i][j]<<" ";
            }
            cout<<"|\n";
        }
}

//return the time it take to run next turn for it number of iterations
auto bench_mark(int**(*func)(int**,int,int),int** world, int m, int n,int it){
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
int ** next_turn(int** world,int m,int n)
{
    int** new_world; 
    new_world = new int*[m];
    for(int i = 0; i<m; i++){
        new_world[i] = new int[n];
        for(int j = 0; j<n; j++){
            new_world[i][j] = 0;
        }
    }
    //cout<< "in next turn";
    
    for(int i = 0;i < m ;i++){ 
        for(int j = 1; j < n-1; j++){
            if(i ==0||i==m-1 || j == 0 || j == n-1){
                new_world[i][j] = 0;
            }else{
                int living = world[i-1][j-1] + world[i-1][j] + world[i-1][j+1] +
                            world[i][j+1] + world[i+1][j-1] + world[i+1][j] +
                            world[i+1][j+1] + world[i][j-1];

                new_world[i][j] = dead_or_alive(living, world[i][j]);
            }
        }
    }
    delete[] world;

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
    vector<int**> test_cases;
    //cout<< "in main before going to create world";
    //create test cases into a vector
    for (int i = 0; i < num_tests; i++){
        test_cases.push_back(create_world(m,n));
    }
    //cout<<"in main before going to next turn";
    //run each case
    for(vector<int**>::iterator i = test_cases.begin(); i != test_cases.end(); ++i){
        time +=bench_mark(next_turn,*i,m,n,iterations);
    }
    //print out the average
    cout<< "ran "<< num_tests << " random games of "<< n-2 << " by "<< m-2 << " for "<< iterations<< " iterations, average time is: "<< time/num_tests<<endl;
    
    /*
    int** world= create_world(m,n);
    cout << "our initial matrix\n";
    print_matrix(world,m,n);
    auto const start_time = std::chrono::steady_clock::now();
    for(int iter = 0; iter < iterations; iter++){// print out each iteration of the matric
        cout<<"\n";
        cout<< iter+1 << "th iteration\n";
        world = next_turn(world,m,n);
        print_matrix(world,m,n);
    }*/
    //bench_mark(next_turn,world,m,n);
    //auto const end_time = std::chrono::steady_clock::now();
    // cout<< std::chrono::duration_cast<std::chrono::microseconds>( end_time - start_time ).count() << " micro seconds\n";
    //auto const avg_map = csc586::benchmark::benchmark(  next_turn,  world );
}