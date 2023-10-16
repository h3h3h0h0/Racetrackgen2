#include "NoiseGenerator.h"

using namespace std;

NoiseGenerator::NoiseGenerator(int l, int w, unsigned int seed): length{l}, width{w}, pn{new PerlinNoise{seed}}, values{new double[length*width]} {
    regenerate(defaultPower, defaultDetail);
}
NoiseGenerator::~NoiseGenerator() {
    delete pn;
    delete [] values;
}

void NoiseGenerator::regenerate(double power, int detail) {
    double minv = DBL_MAX, maxv = DBL_MIN;
    for(int i = 0; i < this->length; i++){
        for(int j = 0; j < this->width; j++) {
            int cind = j+i*(this->width);
            double nx = i*1.0/(this->length) - 0.5, ny = j*1.0/(this->width) - 0.5;
            double e = 0.0;
            for(int k = 0; k < detail; k++) {
                e += pn->noise(pow(2, k)*nx, pow(2, k)*ny, nx*ny);
            }
            values[cind] = pow(e, power);
            minv = min(minv, values[cind]);
            maxv = max(maxv, values[cind]);
        }
    }
    for(int i = 0; i < (this->length)*(this->width); i++) {
        values[i] = (values[i]-minv)/(maxv-minv);
    }
}

int NoiseGenerator::getLength() {return length;}
int NoiseGenerator::getWidth() {return width;}

//DEALLOCING VALUE ARRAY IS RESPONSIBILITY OF USER!
double *NoiseGenerator::getValues() {
    double *ret = new double[this->length*this->width];
    for(int i = 0; i < this->length*this->width; i++) {
        ret[i] = values[i];
    }
    return ret;
}
