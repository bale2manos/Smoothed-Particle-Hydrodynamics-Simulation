//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
#define ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
#include <vector>

struct Particle {
    double px, py, pz, hvx, hvy, hvz, vx, vy, vz;
    int id;
};

struct Block {
    int i, j, k;
    std::vector<Particle> particles;
};
int calculate_block_index(double position, double min_limit, double block_size);
#endif //ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
