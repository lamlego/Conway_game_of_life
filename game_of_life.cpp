#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>

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
bool ** create_world(int m, int n)
{
    bool** world = 0;
    world = new bool*[m];
    srand(time(0));
    for(int i = 0; i < m ; i++){
        world[i] = new bool[n];
        for(int j = 0; j< n; j++){
           world[i][j] = rand()%2;
        }
    }
    return world;
}
//takes a 2d bool matrix and dimentions and print it
void print_matrix(bool** world,int m, int n){
    for (int i = 0; i < m; i++){
            std::cout<<"| ";
            for(int j =0; j < n; j++){
                std::cout<<world[i][j]<<" ";
            }
            std::cout<<"|\n";
        }
}
/**
 * takes a 2d boolean matrix,and the dimentions
 * goes through each cell
 * determine the next frame of the matrix 
 */
bool ** next_turn(bool** world,int m,int n)
{
        bool ** new_world = 0;
        new_world = new bool*[m]; //initialize the first dimention of new_world
        for(int i = 0;i < m;i++){
            new_world[i] = new bool[n];//initialize each horizontal array of new_world
            //std::cout<<"| ";
            for(int j = 0; j < n; j++){
                int living = 0;//use count how many cells are alive surrounding our cell

                //check cell to the left  
                if(i > 0 ){
                    living += world[i-1][j];
                }
                //check cell to the right
                if(i < m-1){
                    living += world[i+1][j];
                }
                //check cell to the top
                if(j > 0){
                    living += world[i][j-1];
                }
                //check cell to the bottom
                if(j < n-1){
                    living += world[i][j+1];
                }
                //check cell to the top left
                if(i > 0 && j > 0){
                    living += world[i-1][j-1];
                }
                //check cell to the bottom right
                if(i< m-1 && j < n-1){
                    living += world[i+1][j+1];
                }
                //check cell to the bottom left
                if(i > 0 && j < n-1){
                    living += world[i-1][j+1];
                }
                //check cell to the top right
                if(i < m-1 && j > 0){
                    living += world[i+1][j-1];
                }
                new_world[i][j] = dead_or_alive(living, world[i][j]);
                //std::cout<< new_world[i][j]<<" ";
            }
            //std::cout<<"|"<< i <<"\n";
        }//std::cout<<"\n";
    return new_world;
}

int main(int argc, char *argv[]){
    //maybe set it up so these variables can be input from user
    if(argc > 4 || argc < 3){
            
        std::cout<<"Please enter 3 arguments";
        return 1;
    }
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    //Checking data that was entered making sure it is INT
    if((m == 0) | (n == 0) | (iterations == 0)){
        std::cout<< "must enter 3 INT values";
        return 0;
    }
    bool** world= create_world(m,n);
    std::cout << "our initial matrix\n";
    print_matrix(world,m,n);
    //
    for(int iter = 0; iter < iterations; iter++){// print out each iteration of the matric
        std::cout<<"\n";
        std::cout<< iter+1 << "th iteration\n";
        world = next_turn(world,m,n);
        print_matrix(world,m,n);
    }
}