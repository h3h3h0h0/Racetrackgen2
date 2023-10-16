#include "PerlinNoise.h"
#include <random>
#include <limits>
#include <math.h>
#include <cfloat>

using namespace std;

//generator for arrays of normalized noise with various specifications
class NoiseGenerator {
    int length, width;
    PerlinNoise *pn; //sometimes may be redeclared for seeding purposes
    double *values;

    const double defaultPower = 1;
    const double defaultDetail = 1;
    
    public:
        NoiseGenerator(int l, int w, unsigned int seed); //pregenerates using default values
        ~NoiseGenerator();
        void regenerate(double power, int detail);
        int getLength();
        int getWidth();
        double *getValues(); //DEALLOCING VALUE ARRAY IS RESPONSIBILITY OF USER!
};