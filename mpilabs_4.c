// $HOME/opt/usr/local/bin/mpicc mpilabs.c -o mpilabs
// $HOME/opt/usr/local/bin/mpirun -n 8 mpilabs
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <mpi.h>
#define pi 3.14159265359

unsigned int countSetBits(int);
int turnOffK(int, int);
int energycalc(int, int);

int main(int argc, char *argv[])
{

    int length = 23;
    int N = length - 1; // only half the search space must be searched

    int rank, k;
    int my_first, my_last;

    MPI_Init (&argc, &argv); //initialize MPI library
    MPI_Comm_size(MPI_COMM_WORLD, &k); //get number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //get my process id

    uint64_t search_space = pow(2, N); // only half of the search space needs to be searched
    uint64_t search_space_per_node = search_space/k; // 2^(N-k) search space per node
    int energy;
    int c; // autocorrelation
    int minenergy = floor((pow(length,2)*pow(8*pi*length, 3/(2*(double)length)))/14.6496);
    int minenergy_global;
    int optimal;
    int comms_rate = 4; // frequency of communication in one search ie. if comms_rate = 4, I want comms every quarter way through
    int counter = 1;

    if(rank==0){
        printf("Length of Sequence = %d\n", length);
    }

    my_first = search_space_per_node * rank;
    my_last = (search_space_per_node*(rank + 1)) - 1;
    
    
    
    for(int count = my_first; count <= my_last; count++){

        if(count == (my_first + ((counter*search_space_per_node)/comms_rate))){

            MPI_Allreduce(&minenergy, &minenergy_global, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);// Communicate minimum number found....
            if(minenergy_global<minenergy){
            minenergy = minenergy_global;
            }
            counter++;
        }

        energy = 0;

        for(int i = 1; i < length; i++){

            int r = length - i;
            uint64_t p = pow(2, r);
            int d = count >> i;
            int x = count^d;

            if(x >= p){
                for(int j = length; j > r; j--){
                    x = turnOffK(x, j);
                    if(x < p){
                        j = length - i + 1;
                    }
                }
            }
            
            int o = countSetBits(x); // number of 1s
            int z = r - o; // number of 0s
            c = z - o;
            energy += pow(c, 2);
            if(energy >= minenergy){ // if the partial energy is greater than the minimum energy already found, stop the calculation for this sequence and go to the next sequence
                i = length - 1;
            }

        } // finish calculating energy
        if(energy < minenergy){ // if the energy of the current sequence is less than minenergy, than minenergy is changed to the current energy
		    optimal = count;
            minenergy = energy;
	    } 

        

	}  

    MPI_Allreduce(&minenergy, &minenergy_global, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    if(rank==0){
        printf("I am rank %d, the global minimum is %d\n", rank, minenergy_global);
    }
    int ans = energycalc(length, optimal);
    if(ans==minenergy_global){
    printf("I am rank %d, my optimal is %d = %d\n", rank, optimal, ans);
    }

    MPI_Finalize();
    return 0;
}

unsigned int countSetBits(int n)
{
	unsigned int count = 0;
	while (n) {
		n &= (n - 1);
		count++;
	}
	return count;
}

int turnOffK(int n, int k)
{
    // k must be greater than 0
    if (k <= 0) return n;
 
    // Do & of n with a number with all set bits except
    // the k'th bit
    return (n & ~(1 << (k - 1)));
}

int energycalc(int length, int count){
    int energy;
    int c; // autocorrelation
  

        energy = 0;

        for(int i = 1; i < length; i++){

            int r = length - i;
            uint64_t p = pow(2, r);
            int d = count >> i;
            int x = count^d;

            if(x >= p){
                for(int j = length; j > r; j--){
                    x = turnOffK(x, j);
                    if(x < p){
                        j = length - i + 1;
                    }
                }
            }
            
            int o = countSetBits(x); // number of 1s
            int z = r - o; // number of 0s
            c = z - o;
            energy += pow(c, 2);

        }
        return energy;

    
}
 
