//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
#define ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
#include <vector>
#include <array>


struct Particle {
    std::array<double,3> p, hv, v, a;
    double rho;
    int id;
    int current_block;
};


struct Block {
    std::array<int,3> coords;             // coords[0] = i, coords[1] = j, coords[2] = k
    std::vector<Particle> particles;
    std::vector<int> neighbours;
};
int initial_block_index(double position, double min_limit, double block_size);
int calculate_block_index(std::array<int,3> coords,int nx, int ny);

std::vector<int> check_neighbours(std::array<int,3> coords, std::array<int,3> n_blocks);
#endif //ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
