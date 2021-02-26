#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <chrono>	 // timing libraries
#include <vector>
#include <algorithm>
#include <bitset>
using namespace std;

/**takes the number of living cell and the current bit
 * returns a boolean
 */
bool dead_or_alive(int living, bool current){
    bool newcell = current;
    //std::cout<< current <<" ";
    if(current == 0){
        if(living == 3){
            newcell = 1;
        }
    }
    else {
       if(living < 2){
           newcell = 0;
       }
       else if (living > 3){
           newcell = 0;
       }
    }
    return newcell;
}
//creates a matrix of m by n and loading cells with 0|1
vector<vector<bool>> create_world(int m, int n)
{
    vector<vector<bool>> world;
    srand(time(0));
    for(int i = 0; i < m ; i++){
        world.push_back(vector<bool>());
        for(int j = 0; j< n; j++){
           world[i].push_back(rand()%2);
        }
    }
    
    return world;
}
//takes a 2d bool matrix and dimentions and print it
void print_matrix(vector<vector<bool>> world,int m, int n){
    for (int i = 0; i < m; i++){
            cout<<"| ";
            for(int j =0; j < n; j++){
                cout<<world[i][j]<<" ";
            }
            cout<<"|\n";
        }
}

//return the time it take to run next turn for it number of iterations
auto bench_mark(vector<vector<bool>>(*func)(vector<vector<bool>>,int,int),vector<vector<bool>> world, int m, int n,int it){
auto const start_time = std::chrono::steady_clock::now();
    for(int i=0; i< it; i++){
        cout<<"\nthis is the "<< i+1 << " iteration\n";
        //print_matrix(world,m,n);
        world = func(world,m,n);
    }
    
auto const end_time = std::chrono::steady_clock::now();
return(std::chrono::duration_cast<std::chrono::microseconds>( end_time - start_time ).count());
//cout<< std::chrono::duration_cast<std::chrono::microseconds>( end_time - start_time ).count() << " micro seconds\n";
}

/**
 * takes a 2d boolean matrix,and the dimentions
 * goes through each cell
 * determine the next frame of the matrix 
 */
vector<vector<bool>> next_turn(vector<vector<bool>> world,int m,int n)
{
        vector<vector<bool>> new_world;
        //new_world = new bool*[m]; //initialize the first dimention of new_world
        for(int i = 0;i < m;i++){
            //new_world[i] = new bool[n];//initialize each horizontal array of new_world
            //std::cout<<"| ";
            vector<bool> col;
            
            for(int j = 0; j < n; j++){
                
                int living = 0;//use count how many cells are alive surrounding our cell
                //check cell to the top left
                if(i > 0 && j > 0){
                    living += world[i-1][j-1];
                }
                //check cell to the left  
                if(i > 0 ){
                    living += world[i-1][j];
                }
                //check cell to the bottom left
                if(i > 0 && j < n-1){
                    living += world[i-1][j+1];
                }
                 //check cell to the top
                if(j > 0){
                    living += world[i][j-1];
                }   
                //check cell to the bottom
                if(j < n-1){
                    living += world[i][j+1];
                }
                //check cell to the top right
                if(i < m-1 && j > 0){
                    living += world[i+1][j-1];
                }
                //check cell to the right
                if(i < m-1){
                    living += world[i+1][j];
                }
                //check cell to the bottom right
                if(i< m-1 && j < n-1){
                    living += world[i+1][j+1];
                }
                //cout<<"living "<< living;
                col.push_back (dead_or_alive(living, world[i][j]));
                //std::cout<< new_world[i][j]<<" ";
            }new_world.push_back(col);
            //std::cout<<"|"<< i <<"\n";
        }//std::cout<<"\n";
    //print_matrix(new_world,m,n);
    return new_world;
}

int main(int argc, char *argv[]){
    if(argc != 5){
        cout<<"Usage: ./gol [width] [height] [iterations] [number of tests](optional)";
        return 1;
    }
    const int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    int num_tests = atoi(argv[4]);
    //Checking data that was entered making sure it is INT
    if((m == 0) | (n == 0) | (iterations == 0) | (num_tests == 0)){
        cout<< "must enter 4 INT values";
        return 0;
    }
    int time;
    vector<vector<vector<bool>>> test_cases;
    //create test cases into a vector
    for (int i = 0; i < num_tests; i++){
        test_cases.push_back(create_world(m,n));
    }
    //cout<<"hi";
    //run each case
    for(vector<vector<vector<bool>>>::iterator i = test_cases.begin(); i != test_cases.end(); ++i){
        time +=bench_mark(next_turn,*i,m,n,iterations);
    }
    //print out the average
    cout<< "ran "<< num_tests << " random games of "<< n << " by "<< m << " for "<< iterations<< " iterations, average time is: "<< time/num_tests<<endl;


    /*vector<vector<bool>> world= create_world(m,n);
    cout << "our initial matrix\n";
    print_matrix(world,m,n);
    //auto const start_time = std::chrono::steady_clock::now();
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