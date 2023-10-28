#include "NoiseGenerator.h"
#include <utility>
#include <tuple> //this will be used for the coordinates of points in 3d space
#include <vector>
#include <cstdlib>
#include <random>
#include <cmath>

using namespace std;

class TrackGenerator {
    NoiseGenerator *ng;
    double ld, wd; //"real" dimensions
    int ls, ws; //number of samples (i.e. dimensions of noise array)
    double *noiseArray;
    const unsigned int attempts = 10; //will attempt this many times at most to generate
    public:
        TrackGenerator(double l, double w, int sl, int sw, unsigned int seed=0);
        ~TrackGenerator();
        void renoise(double power, int detail); //regenerates the noise array
        vector<tuple<double, double, double>> generateRaw(int points, double len, double maxgradient, double mindist, double maxdist);
        vector<tuple<double, double, double>> generateSmooth(int points, double len, double maxgradient, double mindist, double maxdist, int ratio); //interpolates between raw points using bezier curves to "smooth out" corners
};