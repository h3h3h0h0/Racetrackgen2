#include "TrackGenerator.h"

TrackGenerator::TrackGenerator(double l, double w, int sl, int sw, unsigned int seed=0): 
ng{new NoiseGenerator{sl, sw, (seed == 0? (unsigned int)(rand()*4294967295) : seed)}}, 
ld{l}, wd{w}, ls{sl}, ws{sw}, 
noiseArray{ng->getValues()} {}

TrackGenerator::~TrackGenerator() {
    delete [] noiseArray;
    delete ng;
}

void TrackGenerator::renoise(double power, int detail) {
    ng->regenerate(power, detail);
    delete [] noiseArray;
    noiseArray = ng->getValues();
}

vector<tuple<double, double, double>> TrackGenerator::generateRaw(int points, double len, double maxgradient, double mindist, double maxdist) {
    
}

vector<tuple<double, double, double>> TrackGenerator::generateSmooth(int points, double len, double maxgradient, double mindist, double maxdist, int ratio) {
    
}
