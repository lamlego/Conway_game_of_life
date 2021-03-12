#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <sstream>
#include <chrono>	 // timing libraries
#include <iomanip>
#include <numeric>
#include <vector>
#include <algorithm>
#include <bitset>
using namespace std;

//volatile int sink; 

/*
**This object will hold everything now. 
*/
class world{
private:
    bool** cells;
    int width;
    //unsigned int width_bytes;
    int length;
    //unsigned int length_bytes;
public:  
    world(int m, int n);
    ~world(void);
    void set_width(int x) {width = x;}
    int get_width() {return width;}
    void set_length(int y) {length = y;}
    int get_length() {return length;}
    void copy_cells(world &current);
    void set_cell(int m, int n);
    void clear_all();
    void clear_cell(int m, int n);
    void create_world();
    int dead_or_alive(int living, int current);
    int cell_state(int x, int y);
    void next_turn(world &new_world);
    void print_world();
};

world here_we_go(int m, int n, int num_tests){
    world nw(m, n);
    nw.set_length(m);
    nw.set_width(n);
    nw.clear_all();

    nw.create_world();
    return nw;
    
}


//return the time it take to run next turn for it number of iterations
auto bench_mark(int m, int n,int it){
    world first(n, m);
    first.set_length(m);
    first.set_width(n);
    first.clear_all();

    auto const start_time = std::chrono::steady_clock::now();
    for(int i = 0; i < it; i++){
        world new_world(m, n);
        new_world.set_length(m);
        new_world.set_width(n);
        first.next_turn(new_world);
    }
    auto const end_time = std::chrono::steady_clock::now();
    return(std::chrono::duration_cast<std::chrono::microseconds>( end_time - start_time ).count());

}

int main(int argc, char *argv[])
{
    if(argc != 5){
        cout<<"Usage: ./gol [width] [height] [iterations] [number of tests](optional)";
        return 1;
    }
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    int num_tests = atoi(argv[4]);
    //Checking data that was entered making sure it is INT
    if((m == 0) | (n == 0) | (iterations == 0) | (num_tests == 0)){
        cout<< "must enter 4 INT values";
        return 0;
    }
    
    world new_world(m, n);
    new_world.set_length(m);
    new_world.set_width(n);
    new_world.clear_all();

    int time = 0;
    vector<world> test_cases;
    //create test cases into a vector
    for (int i = 0; i < num_tests; i++){
        test_cases.push_back(here_we_go(m, n, num_tests));
    }
    //run each case
    for(vector<world>::iterator i = test_cases.begin(); i != test_cases.end(); ++i){
        time +=bench_mark(m,n,iterations);
    }
    //print out the average
    cout<< "ran "<< num_tests << " random games of "<< n << " by "<< m << " for "<< iterations<< " iterations, average time is: "<< time/num_tests<<endl;
    
}
/*
** world constructor, will take in the parameters from the commandline. 
*/
world::world( int m, int n)
{
    cells = new bool*[m];
    for( int i = 0; i < m; i++){
        for( int j = 0; j < n; j++){
            cells[i] = new bool[n];
        }
    }
}

/*
**in the case that we need to clear all the cells that we had been working with
*/
world::~world(void)
{
    delete[] cells; 
}

/*
**copies the content of one world to a new world 
*/
void world::copy_cells(world &current)
{
    world new_world(get_length(), get_width()); 
    for( int i = 0; i < get_width(); i++){
        for( int j = 0; j< get_length(); j++){
            if(current.cell_state(i, j) == 1){
                set_cell(i,j);
            }else{
                clear_cell(i, j);
            }
        }
    }

}

void world::clear_all(){
    for(int i = 0; i<get_width(); i++){
        for(int j = 0; j<get_length(); j++){
            clear_cell(i,j);
        }
    }
}

int world::dead_or_alive(int living, int current){
    int newcell = current;
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

void world::create_world()
{
    int x, y;
    srand(time(0));
    int initial_length = (get_width() * get_length()) /2;
    
    do{
        x = rand()%get_width();
        y = rand()%get_length();
        
        set_cell(x, y);
        
        initial_length -= 1;
        
    }while (-initial_length);
}

/*
**Wil set the cell passed in to on, or 1
*/
void world::set_cell(int m, int n)
{
    cells[m][n] = 1;

}

/**
 * Will turn the cell passed in to off, or 0
*/
void world::clear_cell(int m, int n)
{
    cells[m][n] = 0;

}

/**
 * Returns the passed in cells current state
*/
int world::cell_state(int m, int n)
{
    return cells[m][n];
}

/**
 * takes a 2d int matrix,and the dimentions
 * goes through each cell
 * determine the next frame of the matrix 
 */
void world::next_turn(world &new_world)
{
    //world new_world(get_width(), get_length());
    //cout<< "- NEXT TIME BB - \n";
        
    for(int i = 0;i < get_width();i++){
        //std::cout<<"| ";
        for(int j = 0; j < get_length(); j++){
            int living = 0;//use count how many cells are alive surrounding our cell
            //check cell to the top left
            if(i > 0 && j > 0){
                living += cells[i-1][j-1];
            }
            //check cell to the left  
            if(i > 0 ){
                living += cells[i-1][j];
            }
            //check cell to the bottom left
            if(i > 0 && j < length-1){
                living += cells[i-1][j+1];
            }
                //check cell to the top
            if(j > 0){
                living += cells[i][j-1];
            }   
            //check cell to the bottom
            if(j < length-1){
                living += cells[i][j+1];
            }
            //check cell to the top right
            if(i < width-1 && j > 0){
                living += cells[i+1][j-1];
            }
            //check cell to the right
            if(i < width-1){
                living += cells[i+1][j];
            }
            //check cell to the bottom right
            if(i< width-1 && j < length-1){
                living += cells[i+1][j+1];
            }
            new_world.cells[i][j] = dead_or_alive(living, cell_state(i, j));
            //std::cout<< new_world.cell_state(i,j)<<" ";
        }
        //std::cout<<"|"<< i <<"\n";
    }
    //return new_world;
    
}

void world::print_world(){
    //cout<<"in printing cell";
    //cout << "length: "<< get_length() << "width: "<< get_width();
    for (int i = 0; i < width; i++){
        for(int j = 0; j <length; j++){
            cout<< cell_state(i,j) << ' ';
        }
        cout<< "\n";
    }
    cout<< "\n";
}