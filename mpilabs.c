#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <mpi.h>

unsigned int countSetBits(int);
int turnOffK(int, int);

int main(int argc, char *argv[])
{

    int length = 5;
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
    int minenergy = 100000;
    int optimal; 
    int num_of_optimal = 0; // int value to store how many optimal sequences have been found

    if(rank==0){
        my_first = 0;
        my_last = search_space_per_node - 1;
        printf("Length of Sequence = %d\n", length);
    }
    else if (rank > 0)
    {
        my_first = search_space_per_node * rank;
        my_last = (search_space_per_node*(rank + 1)) - 1;
    }
    
    
    for(int count = my_first; count <= my_last; count++){

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

        }
        if(energy < minenergy){ // if the energy of the current sequence is less than minenergy, than minenergy is changed to the current energy
		minenergy = energy;
		optimal = count; 
		num_of_optimal = 1;

	    } else if(energy == minenergy){ // if current energy is equal to minenergy
        num_of_optimal++; // increment the number of optimal sequences
        }
	} 

    num_of_optimal = num_of_optimal*2; // to account for the other half of the search space
    if(rank<k){
        printf("I am rank %d, my minimum is %d at %d\n", rank, minenergy, optimal);
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


 