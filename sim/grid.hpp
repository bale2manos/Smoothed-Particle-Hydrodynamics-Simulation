//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_GRID_HPP
#define ARQUITECTURA_DE_COMPUTADORES_GRID_HPP

#include "block.hpp"

#include <cmath>
#include <unordered_map>
#include <vector>

constexpr double radio_multiplier    = 1.695;
constexpr double fluid_density       = 10 * 10 * 10;
constexpr double stiffness_presure   = 3.0;
constexpr double rigidity_collisions = 3 * 10000;
constexpr double damping             = 128.0;
constexpr double viscosity           = 0.4;
constexpr double particle_size       = 2 * 0.0001;
constexpr double timestep            = 0.001;
constexpr double xmax                = 0.065;
constexpr double xmin                = -0.065;
constexpr double ymax                = 0.1;
constexpr double ymin                = -0.08;
constexpr double zmax                = 0.065;
constexpr double zmin                = -0.065;
constexpr double gravity             = -9.8;

constexpr double fifteen            = 15.0;
constexpr double three              = 3.0;
constexpr double half               = 0.5;
constexpr double six                = 6.0;
constexpr int nine                  = 9;
constexpr int wrong_particle_number = -5;

class Malla {
  public:
    Malla(int number_particles, double particles_per_meter);
    Malla() = default;

    std::vector<Block> createFillGrid();
    void insert_particles(char const * in_file);
    void insert_particle_info(std::array<std::array<double, 3>, 3> info, int id, int block_index);
    std::array<int, 3> calculate_block_indexes(std::array<double, 3> positions);
    void particle_repositioning();
    void density_increase_and_transformation();
    void acceleration_transfer();
    void particle_collide_move_and_bound_interact();
    void mallaInteraction(int nts);
    [[nodiscard]] int getNumberParticles() const;
    [[nodiscard]] double getParticlesPerMeter() const;
    [[nodiscard]] double getSmoothingLength() const;
    [[nodiscard]] std::array<int, 3> getNumberBlocks() const;
    [[nodiscard]] std::array<double, 3> getSizeBlocks() const;
    [[nodiscard]] std::vector<Particle> getParticles() const;  // New method to retrieve particles
    static int nx_calc(double xmax, double xmin, double h);
    static int ny_calc(double ymax, double ymin, double h);
    static int nz_calc(double zmax, double zmin, double h);
    static double sx_calc(double xmax, double xmin, int nx);
    static double sy_calc(double ymax, double ymin, int ny);
    static double sz_calc(double zmax, double zmin, int nz);
    [[nodiscard]] double smooth_length_calc() const;
    [[nodiscard]] double particle_mass_calc() const;
    int get_block_index_from_position(std::array<double, 3> position);
    static double limit_of_edge(int eje, int extremo);

    void wall_colissions(Particle & particle, Block & block);
    static void edge_collisions(Particle & particula, int extremo, int eje);
    static void particle_movement(Particle & particle);
    void limits_interaction(Particle & particle, Block & block);
    static void edge_interaction(Particle & particle, int extremo, int eje);

    void acceleration_transfer_calculations(Particle & pivot, Particle & particle2);
    void increase_density(std::array<double, 3> & pivot_coords,
                          std::array<double, 3> & particle2_coords, double & pivot_rho,
                          double & particle2_rho) const;
    [[nodiscard]] double density_transformation(double density) const;

  private:
    int number_particles{};
    double particles_per_meter{};
    std::vector<Block> blocks;
    std::array<int, 3> number_blocks{};
    std::array<double, 3> size_blocks{};
    double smoothing_length{}, particle_mass{};
    std::array<double, 2> accel_constants{};
    std::vector<Particle> particles;
    void add_particles_neighbours();
};

#endif  // ARQUITECTURA_DE_COMPUTADORES_GRID_HPP
