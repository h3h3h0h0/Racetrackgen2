#include <iostream>
#include <fstream>
#include "NoiseGenerator.h"

using namespace std;

int main() {
    NoiseGenerator *ng = new NoiseGenerator(5000, 5000, 17283); //just a test seed
    ng->regenerate(2.1, 6);
    double *test = ng->getValues();
    ofstream tout{"TNoise.ppm"};
    tout<<"P3 5000 5000 255"<<endl;
    for(int i = 0; i < 5000; i++) {
        for(int j = 0; j < 5000; j++) {
            int conv = (int)(test[i*5000+j]*255);
            tout<<conv<<" "<<conv<<" "<<conv<<" ";
        }
        tout<<endl;
    }
    delete ng;
    return 0;
}