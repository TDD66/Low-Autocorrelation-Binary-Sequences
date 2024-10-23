#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

unsigned int countSetBits(int);
int turnOffK(int, int);
void autocorrelation(int);

int main()
{
    int length = 10;
    printf("Length of Sequence = %d\n", length);

    clock_t start_time = clock();
    
    autocorrelation(length);

    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
  	printf("\nDone in %f seconds\n", elapsed_time);
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

void autocorrelation(int length)
{
    uint64_t search_space = 1 << (length - 1);
    int energy;
    int c; // autocorrelation
    int minenergy = 100000;
    int optimal; 
    int num_of_optimal = 0; // int value to store how many optimal sequences have been found
    
    for(int count = 0; count < search_space; count++){

        energy = 0;

        for(int i = 1; i < length; i++){

            int r = length - i;
            uint64_t p = 1 << r;
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
            //  C[i-1] = abs(z-o);
            c = z - o;
            energy += c * c;
            if(energy >= minenergy){
                i = length - 1;
            }

        }
        if(energy < minenergy){ // if the energy of the current sequence is less than minenergy, than minenergy is changed to the current energy
		minenergy = energy;
		optimal = count; // first element of 'optimal' vector is now the current sequence in int eg. for length N=3, 001 = 1
		num_of_optimal = 1;

	    } else if(energy == minenergy){ // if current energy is equal to minenergy
        num_of_optimal++; // increment the number of optimal sequences
        }
	} 
    num_of_optimal = num_of_optimal*2; // to account for the other half of the search space
    printf("Min E = %d\nFirst Optimal Number = %d\nOptimal Sequences = %d", minenergy, optimal, num_of_optimal);
}


