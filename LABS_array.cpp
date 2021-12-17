#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;



float EnergyCalc(int, float*);
float MeritCalc(int, float);
void printSequence(int, float*);
float Autocorrelation(int);
float g;

int main()
{
  int l = 18;   
 // printf("Enter length of sequence: ");
  //scanf("%d", &l);
printf("Length of sequence: %d\n", l);
  Autocorrelation(l); 

	return 0;
}


float EnergyCalc(int length, float sequence[]) {
    float C[length-1], Y[length-1];
    float energy = 0;
    for (int i=0; i<length-1; i++){
        C[i] = 0;
        Y[i] = 0;
    } 

     for (int k=1; k<length; k++) {
		for (int j=0; j<length-k; j++) {
            C[k-1] += sequence[j]*sequence[j+k];
		}
	}

	for (int q = 0; q<length-1; q++){
        Y[q] = C[q]*C[q];
	}

	for (int x = 0; x<length; x++){
        energy += Y[x];
	}
    
  //  cout<<" "<<energy;
    
    return energy;

}

float MeritCalc(int length, float energy){

    float meritfactor = (length*length)/(2*energy);
    return meritfactor;

}

void printSequence(int length, float sequence[])
{
	for (int i = 0; i < length; i++) {
		cout << sequence[i] << " ";
	}
	cout << "\n";
}

float Autocorrelation(int length) {
	
int power = (pow(2, length));
float sequence[length];
float energy[power];
float meritfactor[power];
float minenergy = 1000;
float maxfactor;
vector<int> optimal{0};
int num_of_optimal = 1;

for (int count = 0b0; count < power; count++){

	for (int i = 0; i < length; i++){
		sequence[i] = (count >> i) & 1;

		if(sequence[i]==0){
			sequence[i] = -1;
		}

	}
	//printSequence(length, sequence);
	energy[count] = EnergyCalc(length, sequence);
	meritfactor[count] = MeritCalc(length, energy[count]);

	 if(energy[count] < minenergy){
		minenergy = energy[count];
		optimal[0] = count;
		
	} else if(energy[count] == minenergy){
		optimal.push_back(count);
		num_of_optimal++;
	} 

	}
	float display[num_of_optimal];
	maxfactor = MeritCalc(length, minenergy);
	//printSequence(power, energy);
	//printSequence(power, meritfactor);

	if(num_of_optimal == 1){
		cout << "One optimal solution\n";
		printf("Minimum energy = %.0f\n", minenergy);
		printf("Maximum merit factor = %.3f\n", maxfactor);

		for(int j = 0; j < num_of_optimal; j++){

			for (int k = 0; k < length; k++){
		display[k] = (optimal[j] >> k) & 1;

		if(display[k]==0){
			display[k] = -1;
		}

		}
		printSequence(length, display);
		}	
	} else{
		cout << num_of_optimal << " optimal sequences found\n\n";
		printf("Minimum energy = %.0f\n", minenergy);
		printf("Maximum merit factor = %.3f\n", maxfactor);
		for(int j = 0; j < num_of_optimal; j++){

			for (int k = 0; k < length; k++){
		display[k] = (optimal[j] >> k) & 1;

		if(display[k]==0){
			display[k] = -1;
		}

		}
		printSequence(length, display);
	} 

	cout << "\nEND";

	return 0;
}
}
