#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//first run through of coding the game of life

int dead_or_alive(int living, int current){
    int new_int;
    if (living<2 || living >=3 && current ==1){
        new_int = 0;
    } else if(living ==2 || living ==3 && current == 0){
        new_int = 1;
    }else{
        new_int = current;
    }
    return new_int;
}

int main(){
    //maybe set it up so these variables can be input from user
    int m = 5;
    int n = 5; 

    int world [m][n];

    //loading the matrix with "random" values
    for(int i = 0; i < m ; i++){
        srand(time(NULL));
        for(int j = 0; j< n; j++){
            world[i][j] = rand()%2 ;
        }
    }

    //printing out the matrix
    for (int i = 0; i < m ; i++){
        std::cout<<"\n";
        for(int j =0; j < n; j++){
            std::cout<<world[i][j]<<" ";
        }
    }

    std::cout<<"\n\n";
    
    //updating the matrix
    int living;
    for(int i = 0; i<m; i++){
        for(int j = 0; j<n; j++){
            //count the neighbors who are alive
            if(i==0){
                if (j==0){
                    living = world[i+1][j] + world[i+1][j+1] + world[i][j+1];
                    world[i][j] = dead_or_alive(living, world[i][j]);
                }else if(j==4){
                    living = world[i][j-1] + world[i+1][j] + world[i+1][j-1];
                    world[i][j] = dead_or_alive(living, world[i][j]);
                }else{
                    living = world[i][j-1] + world[i+1][j-1] + world[i+1][j] + world[i+1][j+1] + world[i][j+1];
                    world[i][j] = dead_or_alive(living, world[i][j]);
                }
                
            }else if(i == 4){
                if(j == 0){
                    living = world[i-1][j] + world[i-1][j+1] + world[1][j+1];
                    world[i][j] = dead_or_alive(living, world[i][j]);
                }else if(j == 4){
                    living = world[i][j-1] + world[i-1][j-1] + world[i-1][j];
                    world[i][j] = dead_or_alive(living, world[i][j]);
                }else{
                    living = world[i][j-1] + world[i-1][j-1] + world[i-1][j] + world[i-1][j+1] + world[i+1][j+1];
                    world[i][j] = dead_or_alive(living, world[i][j]);
                }
            }else if(j == 0){
                if(i == 4){
                    living = world[i-1][j] + world[i-1][j+1] + world[i][j+1];
                    world[i][j] = dead_or_alive(living, world[i][j]);
                }else{
                    living = world[i-1][j] + world[i-1][j+1] + world[i][j+1] + world[i+1][j+1] + world[i+1][j];
                    world[i][j] = dead_or_alive(living, world[i][j]);
                }
            }else if(j == 4){
                living = world[i-1][j] + world[i-1][j-1] + world[i][j-1] + world[i+1][j-1] + world[i+1][j];
                world[i][j] = dead_or_alive(living, world[i][j]);
            }else{
                living = world[i-1][j-1] + world[i-1][j] + world[i-1][j+1] + world[i][j+1] + world[i+1][j+1] + world[i+1][j] + world[i+1][j-1] + world[i][j-1];
                world[i][j] = dead_or_alive(living, world[i][j]);
            }
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