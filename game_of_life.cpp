#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>

//first run through of coding the game of life

int dead_or_alive(int living, int current){
    int new_int;
    if ((living<2 || living >3) && current == 1){
        new_int = 0;
    } else if((living ==2 || living ==3) && current == 1){
        new_int = 1;
    }else if(current == 0 && living == 3){
        new_int = 1;
    }else{
        new_int = current;
    }
    return new_int;
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
    int world [m][n];
    int new_world [m][n];

    //Checking data that was entered making sure it is INT
    if((m == 0) | (n == 0) | (iterations == 0)){
        std::cout<< "must enter 3 INT values";
        return 0;
    }
    

    //loading the matrix with "random" values
    for(int i = 0; i < m ; i++){
        srand(time(NULL));
        for(int j = 0; j< n; j++){
            world[i][j] = rand()%2 ;
        }
    }

    //printing out the matrix
    std::cout << "our *random* matrix\n";
    for (int i = 0; i < m ; i++){
        std::cout<<"\n";
        for(int j =0; j < n; j++){
            std::cout<<world[i][j]<<" ";
        }
    }
    for(int iter = 0; iter < iterations; iter++){
        std::cout<<"\n\n";
        std::cout<< iter << "th iteration\n";
        
        //updating the matrix
        int living = 0;


        for (int i = 0; i < m ; i++){
            for(int j = 0; j <n; j++){

                if (i ==0 ){
                    //edge case 1
                    if(j == 0){
                        //edge case 1.1
                        living = world[i+1][j] + world[i+1][j+1] + world[i][j+1];
                        new_world[i][j] = dead_or_alive(living, world[i][j]);
                        //std::cout<<living << " ";
                        living = 0;
                    }else if (j == 4){
                        //edge case 1.2
                        living = world[i][j-1] + world[i+1][j-1] + world[i+1][j];
                        new_world[i][j] = dead_or_alive(living, world[i][j]);
                        //std::cout<<living << " ";
                        living = 0;
                    }else{
                        //base case 1
                        living = world[i][j-1] + world[i+1][j-1] + world[i+1][j] + world[i+1][j+1] + world[i][j+1];
                        new_world[i][j] = dead_or_alive(living, world[i][j]);
                        //std::cout<<living << " ";
                        living = 0;
                    }
                }else if (i == 4){
                    //edge case 2
                    if(j==0){
                        //edge case 2.1
                        living = world[i-1][j] + world[i-1][j+1] + world[i][j+1];
                        new_world[i][j] = dead_or_alive(living, world[i][j]);
                        //std::cout<<living << " ";
                        living = 0;
                    }else if(j == 4){
                        //edge case 2.2
                        living = world[i][j-1] + world[i-1][j-1] + world[i-1][j];
                        new_world[i][j] = dead_or_alive(living, world[i][j]);
                        //std::cout<<living << " ";
                        living = 0;
                    }else{
                        //base case 2
                        living = world[i][j-1] + world[i-1][j-1] + world[i-1][j] + world[i-1][j+1] + world[i][j+1];
                        new_world[i][j] = dead_or_alive(living, world[i][j]);
                        //std::cout<<living << " ";
                        living = 0;
                    }
                }else if(j == 0){
                    //edge case 3
                    //we have already covered the cases where i = 0 and i = 4
                    living = world[i-1][j] + world[i-1][j+1] + world[i][j+1] + world[i+1][j+1] + world[i+1][j];
                    new_world[i][j] = dead_or_alive(living, world[i][j]);
                    //std::cout<<living << " ";
                    living = 0;

                }else if(j == 4){
                    //edge case 4
                    //already covered i =0 and i=4
                    living = world[i-1][j] + world[i-1][j-1] + world[i][j-1] + world[i+1][j-1] + world[i+1][j];
                    new_world[i][j] = dead_or_alive(living, world[i][j]);
                    //std::cout<<living << " ";
                    living = 0;
                }else{
                    //base case
                    //all the middle cell spots are going to be in this case. 
                    living = world[i-1][j-1] + world[i-1][j] + world[i-1][j+1] + world[i][j+1] + world[i+1][j+1] + world[i+1][j] + world[i+1][j-1] + world[i][j-1];
                    new_world[i][j] = dead_or_alive(living, world[i][j]);
                    //std::cout<<living << " ";
                    living = 0;
                }
            }
        }

        //updating the matrix
        for(int i = 0; i<m; i++){
            for(int j = 0; j<n; j++){
                world[i][j] = new_world[i][j];
            }
        }

        //printing out the matrix
        for (int i = 0; i < m ; i++){
            std::cout<<"\n";
            for(int j =0; j < n; j++){
                std::cout<<world[i][j]<<" ";
            }
        }
    }

}