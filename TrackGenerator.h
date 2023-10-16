#include "NoiseGenerator.h"
#include <utility>
#include <tuple> //this will be used for the coordinates of points in 3d space
#include <vector>

using namespace std;

class TrackGenerator {
    NoiseGenerator *ng;
    double ld, wd; //"real" dimensions
    int ls, ws; //number of samples (i.e. dimensions of noise array)
    double *noiseArray;
    public:
        TrackGenerator(double l, double w, int sl, int sw);
        ~TrackGenerator();
        void renoise(double power, int detail); //regenerates the noise array
        vector<tuple<double, double, double>> generateRaw(int points, double len, double maxgradient);
        vector<tuple<double, double, double>> generateSmooth(int points, double len, double maxgradient, int ratio); //interpolates between raw points using bezier curves to "smooth out" corners
};