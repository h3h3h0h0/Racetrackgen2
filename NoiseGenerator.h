#include "PerlinNoise.h"
#include <random>
#include <limits>
#include <math.h>
#include <cfloat>

using namespace std;

class NoiseGenerator {
    int length, width;
    PerlinNoise *pn; //sometimes may be redeclared for seeding purposes
    double *values;
    public:
        NoiseGenerator(int l, int w, unsigned int seed); //pregenerates using default values
        ~NoiseGenerator();
        void regenerate(double power, int detail);
        int getLength();
        int getWidth();
        double *getValues(); //DEALLOCING VALUE ARRAY IS RESPONSIBILITY OF USER!
};