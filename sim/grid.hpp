//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_GRID_HPP
#define ARQUITECTURA_DE_COMPUTADORES_GRID_HPP

#include <vector>
#include "block.hpp"
#include <cmath>
#include <unordered_map>


using namespace std;

constexpr double radio = 1.695;
constexpr double density_factor = 10*10*10;
constexpr double p_s = 3.0; //presión de rigidez
constexpr double s_c = 3*10000;
constexpr double d_v = 128.0;
constexpr double viscosity = 0.4;
constexpr double d_p = 2 * 0.0001;
constexpr double delta_t = 0.001;
constexpr double xmax = 0.065;
constexpr double xmin = -0.065;
constexpr double ymax = 0.1;
constexpr double ymin = -0.08;
constexpr double zmax = 0.065;
constexpr double zmin = -0.065;
constexpr double gravity = -9.8;

constexpr double fifteen = 15.0;
constexpr double three = 3.0;
constexpr double half = 0.5;
constexpr double six = 6.0;


class Malla {
  public:
    Malla(int np, double ppm);

    vector<Block> createFillGrid();
    void insert_particles (const char * in_file);
    void insert_particle_info(std::array<std::array<double, 3>, 3> info, int id, int block_index);
    array<int, 3> calculate_block_indexes(std::array <double,3> positions);
    void repos();
    void densinc();
    void acctransf();
    void triplete();
    void mallaInteraction();
    [[nodiscard]] int getNp() const;
    [[nodiscard]] double getPpm() const;
    [[nodiscard]] double getH() const;
    [[nodiscard]] std::array<int, 3>  getNBlocks() const;
    [[nodiscard]] std::array<double, 3>  getSizeBlocks() const;
    [[nodiscard]] std::vector<Particle> getParticles() const; // New method to retrieve particles
    static int nx_calc (double xmax, double xmin, double h);
    static int ny_calc (double ymax, double ymin, double h);
    static int nz_calc (double zmax, double zmin, double h);
    static double sx_calc (double xmax, double xmin, int nx);
    static double sy_calc (double ymax, double ymin, int ny);
    static double sz_calc (double zmax, double zmin, int nz);
    [[nodiscard]] double smooth_length () const;
    [[nodiscard]] double particle_mass () const;
    int get_block_index_from_position (std::array<double, 3> position);


    void wall_colissions(Particle& particle, Block& block);
    static void edge_collisions(Particle& particula, int extremo, int eje);
    static void particle_movement(Particle& particle);
    void limits_interaction(Particle& particle, Block& block);
    static void edge_interaction(Particle& particle,int extremo,int eje);


    void acceleration_transfer(Particle & pivot, Particle & particle2) ;
    void increase_density(std::array<double, 3>& pivot_coords, std::array<double, 3>& particle2_coords, double& pivot_rho, double& particle2_rho) const;
    [[nodiscard]] double density_transformation(double rho) const;
  private:
    int np;
    double ppm;
    std::vector<Block> blocks;
    std::array<int, 3> nBlocks;
    std::array<double, 3> sizeBlocks;
    double h, m;
    std::array<double, 2> accConst;
    std::vector<Particle> particles;
    void add_particles_neighbours();
};

#endif //ARQUITECTURA_DE_COMPUTADORES_GRID_HPP
