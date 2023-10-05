//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
#define ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
#include <vector>
#include <array>

struct Particle1 {
    double px, py, pz, hvx, hvy, hvz, vx, vy, vz, ax, ay, az, rho;
    int id;
};
struct Particle {
    std::array<double,3> p, hv, v, a;
    double rho;
    int id;
};



struct Block {
    int i, j, k;
    std::vector<Particle> particles;
    std::vector<int> neighbours;
};
int initial_block_index(double position, double min_limit, double block_size);

std::vector<int> check_neighbours(int i, int j, int k, double nx, double ny, double nz);
#endif //ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
