#include "NoiseGenerator.h"

using namespace std;

NoiseGenerator::NoiseGenerator(int l, int w, unsigned int seed): length{l}, width{w}, pn{new PerlinNoise{seed}}, values{new double[length*width]} {
    regenerate(1, 1, 1);
}

void NoiseGenerator::regenerate(double xf, double yf, double pow) {
    uniform_real_distribution<double> unif(DBL_MIN, DBL_MAX);
    default_random_engine re;
    double zval = unif(re);
    for(int i = 0; i < length; i++){
        for(int j = 0; k < width; j++) {
            int cind = j+i*width;
            double nx = i/length - 0.5, ny = j/width - 0.5;
            values[cind] = pow(pn->noise(nx*xf, ny*yf, zval), pow);
        }
    }
}

int NoiseGenerator::getLength() {return length;}
int NoiseGenerator::getWidth() {return width;}

//DEALLOCING VALUE ARRAY IS RESPONSIBILITY OF USER!
double NoiseGenerator::*getValues() {
    double *ret = new double[length*width];
    for(int i = 0; i < length*width; i++) {
        ret[i] = values[i];
    }
    return ret;
}
