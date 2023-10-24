#include "TrackGenerator.h"
#include <random>

using namespace std;

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

//will call this repeatedly until it is not stuck
//if stuck, returns empty vector
vector<tuple<double, double, double>> genWS(vector<pair<double, double>> &pts, int start, double len, double maxgradient, double mindist, double maxdist) {

}

//will not go over min/max distances between nodes unless ABSOLUTELY NECESSARY
vector<tuple<double, double, double>> TrackGenerator::generateRaw(int points, double len, double maxgradient, double mindist, double maxdist) {
    //vector of points (in 2D)
    vector<pair<double, double>> pts;
    //random generation stuff
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> disx(0, ld);
    uniform_real_distribution<> disy(0, wd);
    //generate points with x, y
    for(int i = 0; i < points; i++) {
        double cx = disx(gen);
        double cy = disy(gen);
        pts.push_back(pair{cx, cy});
    }
    //choose start
    uniform_int_distribution<> diss(0, pts.size()-1);
    vector<tuple<double, double, double>> v;
    for(int i = 0; i < attempts; i++) {
        v = genWS(pts, diss(gen), len, maxgradient, mindist, maxdist);
        if(v.size() != 0) return v;
    }
    return v;
}

vector<tuple<double, double, double>> TrackGenerator::generateSmooth(int points, double len, double maxgradient, double mindist, double maxdist, int ratio) {
    
}
