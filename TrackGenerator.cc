#include "TrackGenerator.h"

using namespace std;

TrackGenerator::TrackGenerator(double l, double w, int sl, int sw, double hs, unsigned int seed=0): 
ng{new NoiseGenerator{sl, sw, (seed == 0? (unsigned int)(rand()*4294967295) : seed)}}, 
ld{l}, wd{w}, ls{sl}, ws{sw}, 
noiseArray{ng->getValues()}, heightScale{(hs > 0)? hs : 1} {}

TrackGenerator::~TrackGenerator() {
    delete [] noiseArray;
    delete ng;
}

void TrackGenerator::renoise(double power, int detail) {
    ng->regenerate(power, detail);
    delete [] noiseArray;
    noiseArray = ng->getValues();
}

void TrackGenerator::setHS(double nhs) {
    heightScale = (nhs > 0)? nhs : heightScale;
}

//line segment intersection checking (most efficient way I could find)
bool ccw(pair<double, double> a, pair<double, double> b, pair<double, double> c) {
    return (c.second-a.second)*(b.first-a.first) > (b.second-a.second)*(c.first-a.first);
}
bool segIntersect(pair<double, double> p11, pair<double, double> p12, pair<double, double> p21, pair<double, double> p22) {
    return ccw(p11, p21, p22) != ccw(p12, p21, p22) && ccw(p11, p12, p21) != ccw(p11, p12, p22);
}

//will call this repeatedly until it is not stuck
//if stuck, returns empty vector
vector<tuple<double, double, double>> TrackGenerator::genWS(vector<pair<double, double>> &pts, int start, double len, double maxgradient, double mindist, double maxdist) {
    vector<int> order;
    vector<double> heights;
    vector<bool> vis;
    for(int i = 0; i < pts.size(); i++) vis.push_back(false);
    order.push_back(start);
    vis[start] = true;
    double totaldist = 0;
    //current x and y
    double cx = pts[start].first;
    double cy = pts[start].second;
    double cxs = cx*ls/ld;
    double cys = cy*ws/wd;
    int crp = cxs*ws+cys;
    if(crp >= ls*ws) crp = ls*ws-1;
    double ch = noiseArray[crp]*heightScale;
    heights.push_back(ch);
    while(totaldist < len) {
        //potentially these points get added in
        vector<int> candidates;
        vector<double> candH; //heights
        vector<double> candD; //distances
        for(int i = 0; i < pts.size(); i++) {
            //check visited
            if(vis[i]) continue;
            double px = pts[i].first;
            double py = pts[i].second;
            //check distance between the specified range
            double dist = sqrt((cx-px)*(cx-px)+(cy-py)*(cy-py));
            if(dist < mindist || dist > maxdist) continue;
            //check path crossing (very inefficient!)
            //this is to make sure the track does not cross over itself
            bool inter = false;
            for(int j = 0; j < order.size()-1; j++) {
                if(segIntersect(pts[j], pts[j+1], make_pair(cx, cy), make_pair(px, py))) {
                    inter = true;
                    break;
                }
            }
            if(inter) continue;
            //check gradients
            //generate scaled x/y values
            double pxs = px*ls/ld;
            double pys = py*ws/wd;
            //generate points for polling noise array
            int prp = pxs*ws+pys;
            //take nearest point for each
            if(prp >= ls*ws) prp = ls*ws-1;
            //get heights
            double ph = noiseArray[prp]*heightScale;
            double hd = abs(ph-ch);
            double gradient = hd/dist;
            if(gradient > maxgradient) continue;
            //finally, we can push back a potential candidate
            candidates.push_back(i);
            candH.push_back(ph);
            candD.push_back(dist);
        }
        if(candidates.empty()) break;
        int ci = rand()*candidates.size();
        order.push_back(candidates[ci]);
        vis[candidates[ci]] = true;
        heights[candidates[ci]] = candH[ci];
        totaldist += candD[ci];
        cx = pts[candidates[ci]].first;
        cy = pts[candidates[ci]].second;
        ch = candH[ci];
    }
    vector<tuple<double, double, double>> fin;
    if(totaldist < len) return fin; //it got stuck at some point
    for(int i = 0; i < order.size(); i++) {
        fin.push_back(make_tuple(pts[order[i]].first, pts[order[i]].second, heights[i]));
    }
    return fin;
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
        pts.push_back(make_pair(cx, cy));
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
