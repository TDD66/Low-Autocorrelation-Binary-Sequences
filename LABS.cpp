#include <iostream>
#include <bits/stdc++.h>

using namespace std;


float EnergyCalc(int, float*);
float MeritCalc(int, float);
void printSequence(float*, int);
void generateBinarySequences(int, float*, int);

int main()
{
  int l = 3;  
  float size[l]; 
  generateBinarySequences(l, size, 0);  



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
    
    cout<<" "<<energy;
    
    return energy;

}

float MeritCalc(int length, float energy){

    float meritfactor = (length*length)/(2*energy);
    cout<<" \n"<<meritfactor;
    return meritfactor;

}

void printSequence(float sequence[], int length)
{
	for (int i = 0; i < length; i++) {
		cout << sequence[i] << " ";
	}
	cout << "\n";
}

void generateBinarySequences(int length, float sequence[], int z)
{
	 if (z == length) {
		printSequence(sequence, length);
		return;
	} 
	sequence[z] = -1;
	generateBinarySequences(length, sequence, z + 1);

	sequence[z] = 1;
	generateBinarySequences(length, sequence, z + 1);
    
}
    
