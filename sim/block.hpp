//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
#define ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
#include <array>
#include <vector>

struct Particle {
    std::array<double, 3> h_vel, pos, vel, acc;
    double density;
    int id;
    int current_block;
};

class Block {
  public:
    Block(std::array<int, 3> const & block_coords, std::array<int, 3> const & n_blocks);

  private:
    std::vector<int> check_neighbours(std::array<int, 3> const & n_blocks);

  public:
    std::array<int, 3> coords;
    std::vector<int> particles;
    std::vector<int> neighbours;
    std::vector<int> neighbours_particles;
};

int initial_block_index(double position, double min_limit, double block_size);
int calculate_block_index(std::array<int, 3> coords, int nx, int ny);
#endif  // ARQUITECTURA_DE_COMPUTADORES_BLOCK_HPP
