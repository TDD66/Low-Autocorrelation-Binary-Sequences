#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

float EnergyCalc(int, float*); // function to calculate the energy of a sequence
float MeritCalc(int, float); // function to calculate merit factor from energy
void printSequence(int, int, float*); // function to easily print sequences
void Autocorrelation(int); // function to find sequences with lowest energy

int main()
{
    clock_t start_time = clock();

    int N = 6; // length of sequence 
	printf("Length of sequence: %d\n", N);

    Autocorrelation(N); 

	double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
  	printf("\nDone in %f seconds\n", elapsed_time);

	return 0;
}

float EnergyCalc(int length, float sequence[]) { // function arguments are length and the sequence
    float C[length-1];
	for(int i = 0; i < length-1; i++){
		C[i] = 0;
	} 
	float Y[length-1];
	for(int i = 0; i < length-1; i++){
		Y[i] = 0;
	} 
    float energy = 0;

     for (int k=1; k<length; k++) {
		for (int j=0; j<length-k; j++) {
            C[k-1] += sequence[j]*sequence[j+k];
		}
	}

	for (int q = 0; q<length-1; q++){
        Y[q] = C[q]*C[q];
	}

	for (int x = 0; x<length-1; x++){
        energy += Y[x];
	}
    
    return energy;

}

float MeritCalc(int length, float energy){

    float meritfactor = (length*length)/(2*energy); // Merit factor equation
    return meritfactor;

}

void printSequence(int decimal, int length, float sequence[]){
    if(decimal == 0){
	    for (int i = 0; i < length; i++) {
		    printf("%.0f ", sequence[i]);
	    }
	    printf("\n");
    }else {
        for (int i = 0; i < length; i++) {
		    printf("%.3f ", sequence[i]);
	    }
	    printf("\n");
        }
}

void Autocorrelation(int length) { 
	
int search_space = (pow(2, length)); // search space
float sequence[length];
float energy[search_space]; // array to store energy values for every sequence
float minenergy = 1000; // float value to hold minimum energy value
float maxfactor; // float value to hold maximum merit factor value
int optimal[search_space]; //  vector to store optimal sequences
for(int i = 0; i < search_space; i++){
			optimal[i] = 0;
		}
int num_of_optimal = 1; // int value to store how many optimal sequences have been found

for (int count = 0; count < search_space; count++){ // this loop convert and int number into a array of 1s and 0s, then 0s into -1s

	for (int i = 0; i < length; i++){
		sequence[i] = (count >> i) & 1;

		if(sequence[i]==0){ // changes the 0s in -1s for calculation purposes
			sequence[i] = -1;
		}

	}
	energy[count] = EnergyCalc(length, sequence); // calculate the energy of a given sequence

	 if(energy[count] < minenergy){ // if the energy of the current sequence is less than minenergy, than minenergy is changed to the current energy
		minenergy = energy[count];
		optimal[0] = count; // first element of 'optimal' vector is now the current sequence in int eg. for length N=3, 001 = 1
		
	} else if(energy[count] == minenergy){ // if current energy is equal to minenergy
        num_of_optimal++; // increment the number of optimal sequences
		optimal[num_of_optimal-1] = count; // add that sequence to the 'optimal' vector in int
		
	} 

} // exit loop when count = search_space(2^N), eg. for length N=3, 111 = 7 is maximum number, so when count = 8, exit the loop

float display[length]; // declare array thats the size of the number of optimal sequences
maxfactor = MeritCalc(length, minenergy); // calculate maximum merit factor from minimum energy


printf("%d optimal sequences found\n\n", num_of_optimal);
printf("Minimum energy = %.0f\n", minenergy);
printf("Maximum merit factor = %.3f\n", maxfactor);
	for(int j = 0; j < num_of_optimal; j++){

		if(optimal[j] != 0){
			
			for (int k = 0; k < length; k++){
					display[k] = (optimal[j] >> k) & 1;

				if(display[k]==0){
					display[k] = -1;
				}
			}

}
	printSequence(0, length, display); 
	} 
printf("\nEND\n");
printSequence(0, search_space, energy);



}
