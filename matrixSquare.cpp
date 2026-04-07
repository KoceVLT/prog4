#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

struct Pt { double x, y, z; };
struct Fig { Pt v[4]; };

void moveR(Fig &f) {
    for (int i = 0; i < 4; i++) f.v[i].x += 2.0;
}

void rot45(Fig &f) {
    double r = 45.0 * M_PI / 180.0;
    double s = sin(r), c = cos(r);
    double mx = 0, my = 0;
    for (int i = 0; i < 4; i++) { mx += f.v[i].x; my += f.v[i].y; }
    mx /= 4.0; my /= 4.0;
    for (int i = 0; i < 4; i++) {
        double dx = f.v[i].x - mx, dy = f.v[i].y - my;
        f.v[i].x = dx * c - dy * s + mx;
        f.v[i].y = dx * s + dy * c + my;
    }
}

void scale(Fig &f) {
    double fac = 2.0;
    double mx = 0, my = 0, mz = 0;
    for (int i = 0; i < 4; i++) { mx += f.v[i].x; my += f.v[i].y; mz += f.v[i].z; }
    mx /= 4.0; my /= 4.0; mz /= 4.0;
    for (int i = 0; i < 4; i++) {
        f.v[i].x = mx + (f.v[i].x - mx) * fac;
        f.v[i].y = my + (f.v[i].y - my) * fac;
        f.v[i].z = mz + (f.v[i].z - mz) * fac;
    }
}

int main() {
    ifstream in("file.txt");
    vector<Pt> pts;
    string ln;
    while (getline(in, ln)) {
        stringstream ss(ln);
        string t; double x, y, z;
        if (ss >> t >> x >> y >> z && t == "v") pts.push_back({x, y, z});
    }
    in.close();

    if (pts.size() >= 4) {
        Fig f;
        for (int i = 0; i < 4; i++) f.v[i] = pts[i];
        
        moveR(f);
        rot45(f);
        scale(f);

        for (int i = 0; i < 4; i++) {
            cout << f.v[i].x << " " << f.v[i].y << " " << f.v[i].z << endl;
        }
    }
    return 0;
}
