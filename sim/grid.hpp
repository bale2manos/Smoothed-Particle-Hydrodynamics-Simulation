//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_GRID_HPP
#define ARQUITECTURA_DE_COMPUTADORES_GRID_HPP

#include <vector>
using namespace std;

struct Particle {
    double px, py, pz, hvx, hvy, hvz, vx, vy, vz;
    int i, j, k;
};

struct Malla {
    int np; float ppm; vector<Particle> particles;
};

#endif //ARQUITECTURA_DE_COMPUTADORES_GRID_HPP
