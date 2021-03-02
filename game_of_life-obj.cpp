#include <stdlib.h>
#include <stdio.h>
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

volatile int sink; 

/*
**This object will hold everything now. 
*/
class world{
private:
    char *cells; 
    int width;
    unsigned int width_bytes;
    int length;
    unsigned int length_bytes;
public:
    world(int m, int n);
    ~world(void);
    void create_world(int m, int n);
    void copy_cells(world &current);
    void set_cell(int x, int y);
    void clear_cell(int x, int y);
    int cell_state(int x, int y);
    void next_turn(world& new_world);
    void print_world(world& current, int m, int n);
};

void world::create_world(int m, int n)
{
    world current(m, n);
    srand(time(0));
    
    for(int i = 0; i < m ; i++){
        for(int j = 0; j< n; j++){
            int random = rand()%2;
            if (random == 1){
                current.set_cell(i,j);
            }else{
                current.clear_cell(i,j);
            }
        }
    }
}
/*
//return the time it take to run next turn for it number of iterations
auto bench_mark(void(*func)(world), int m, int n,int it){
    auto const start_time = std::chrono::steady_clock::now();
    for(int i = 0; i < it; i++){
        world new_world(m, n);
        new_world.func(m,n);
    }
    auto const end_time = std::chrono::steady_clock::now();
    return(std::chrono::duration_cast<std::chrono::microseconds>( end_time - start_time ).count());

}
*/
int main(int argc, char *argv[])
{
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
    
    world nw(m, n);
    nw.create_world(m, n);
    world next(m,n);
    nw.print_world(nw, m, n);
    nw.next_turn(next);
    next.print_world(next, m, n);
    
    }
    /*
    vector<int**> test_cases;
    //create test cases into a vector
    for (int i = 0; i < num_tests; i++){
        test_cases.push_back(new_world.create_world(m,n));
    }
    //run each case
    for(vector<int**>::iterator i = test_cases.begin(); i != test_cases.end(); ++i){
        time +=bench_mark(new_world.next_turn(new_world),*i,m,n,iterations);
    }
    //print out the average
    cout<< "ran "<< num_tests << " random games of "<< n << " by "<< m << " for "<< iterations<< " iterations, average time is: "<< time/num_tests<<endl;
    
}*/
/*
** world constructor, will take in the parameters from the commandline. 
*/
world::world(int m, int n)
{
    width = m;
    length = n;
    width_bytes = (m + 7) / 8;
    length_bytes = width_bytes *m;
    cells = new char[length_bytes];
    //somehing here to clear all cells to start
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
    memcpy(cells, current.cells, length_bytes);
}

/*
**Wil set the cell passed in to on, or 1
*/
void world::set_cell(int m, int n)
{
    char *cell_ptr = cells + (n * width_bytes) + (m / 8);
    *(cell_ptr) &= ~(0x80 >> (m &0x07));
}

/**
 * Will turn the cell passed in to off, or 0
*/
void world::clear_cell(int m, int n)
{
    char *cell_ptr = cells + (n * width_bytes) + ( m/8);
    *(cell_ptr) &= ~(0x80 >> (m & 0x07));
}

/**
 * Returns the passed in cells current state
*/
int world::cell_state(int m, int n)
{
    char* cell_ptr; 
    //un comment this clock if we wna to wrap the edges
    /*
    while(m<0) m += width;
    while(m>= width) m -= width;
    while(n<0) y+= length;
    while(y >= length) y-= length;
    */
   //comment out this code if we want to wrap edges
   if((m<0) || (m> width) || (n<0) || (n>= length)){
       return 0;
   }

   cell_ptr = cells + (n * width_bytes) + (m/8);
   return (*cell_ptr & (0x80>> (m &0x07))) ? 1:0;
}


/**
 * takes a 2d int matrix,and the dimentions
 * goes through each cell
 * determine the next frame of the matrix 
 */
void world::next_turn(world& new_world)
{
        unsigned int living;

        for(int i = 0;i < length;i++){

            for(int j = 0; j < width; j++){
                living = cell_state(j-1, i-1) + cell_state(j, i-1)+
                    cell_state(j+1, i-1) + cell_state(j-1, i) +
                    cell_state(j+1, i) + cell_state(j -1, i +1) + 
                    cell_state(j, i+1) + cell_state(j+1, i+1);
                if(cell_state(j,i) == 1){
                    if ((living != 2) && (living != 3)){
                        new_world.clear_cell(j, i);

                    }
                }else{
                    if (living == 3){
                        new_world.set_cell(j, i);
                    }
                }
                //std::cout<< new_world[i][j]<<" ";
            }
            //std::cout<<"|"<< i <<"\n";
        }
}

void world::print_world(world &current, int m, int n){
    for (int i = 0; i < m; i++){
        cout<< "\n";
        for(int j = 0; j <n; j++){
            cout<< current.cell_state(m,n) << ' ';
        }
    }
    cout<< "\n";
}