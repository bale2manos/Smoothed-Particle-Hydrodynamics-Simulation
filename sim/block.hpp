//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
#define ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
#include <vector>
#include <array>


struct Particle {
    std::array<double,3> hv, p, v, a;
    double rho;
    int id;
    int current_block;
};


class Block {
  public:
    Block(const std::array<int, 3>& block_coords, const std::array<int, 3>& n_blocks);


  private:
    std::vector<int> check_neighbours(const std::array<int, 3>& n_blocks);

  public:
    std::array<int, 3> coords;
    std::vector<int> particles;
    std::vector<int> neighbours;
    std::vector<int> neighbours_particles;
};





int initial_block_index(double position, double min_limit, double block_size);
int calculate_block_index(std::array<int,3> coords,int nx, int ny);

std::vector<int> check_neighbours(std::array<int,3> coords, std::array<int,3> n_blocks);
#endif //ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
