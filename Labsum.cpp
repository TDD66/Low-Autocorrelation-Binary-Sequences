#include <iostream>

using namespace std;

float EnergyCalc(int, float*);
float MeritCalc(int, float);

int main()
{
  int N = 13;
  float S[] = {1,1,1,1,1,-1,-1,1,1,-1,1,-1,1};

  float S_energy = EnergyCalc(N, S);
  MeritCalc(N, S_energy);

	return 0;
}


float EnergyCalc(int length, float sequence[]) {
    float C[length-1], Y[length-1];
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

	float energy = 0;

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