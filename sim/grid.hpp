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
double const radio = 1.695;
double const density_factor = 10*10*10;
double const p_s = 3.0; //presión de rigidez
double const s_c = 3*10000;
double const d_v = 128.0;
double const viscosity = 0.4;
double const d_p = 2 * 0.0001;
double const delta_t = 0.001;
double const xmax = 0.065;
double const xmin = -0.065;
double const ymax = 0.1;
double const ymin = -0.08;
double const zmax = 0.065;
double const zmin = -0.065;
double const gravity = -9.8;

class Malla {
  public:
    Malla(int np, double ppm);

    vector<Block> createFillGrid();
    void insert_particles (const char * in_file);
    void insert_particle_info(array<array<double, 3>, 3> info, int id, int block_index);
    array<int, 3> calculate_block_indexes(array <double,3> positions);
    void repos();
    void densinc();
    void acctransf();
    void triplete();
    void mallaInteraction();
    [[nodiscard]] int getNp() const;
    [[nodiscard]] double getPpm() const;
    [[nodiscard]] double getH() const;
    [[nodiscard]] array<int, 3>  getNBlocks() const;
    [[nodiscard]] array<double, 3>  getSizeBlocks() const;
    [[nodiscard]] std::vector<Particle> getParticles() const; // New method to retrieve particles
    static int nx_calc (double xmax, double xmin, double h);
    static int ny_calc (double ymax, double ymin, double h);
    static int nz_calc (double zmax, double zmin, double h);
    static double sx_calc (double xmax, double xmin, int nx);
    static double sy_calc (double ymax, double ymin, int ny);
    static double sz_calc (double zmax, double zmin, int nz);
    [[nodiscard]] double smooth_length () const;
    [[nodiscard]] double particle_mass () const;


    void wall_colissions(Particle& particle, Block& block);
    static void edge_collisions(Particle& particula, int extremo, int eje);
    static void particle_movement(Particle& particle);
    void limits_interaction(Particle& particle, Block& block);
    static void edge_interaction(Particle& particle,int extremo,int eje);


    void acceleration_transfer(Particle & pivot, Particle & particle2) ;
    void increase_density(array<double, 3>& pivot_coords, array<double, 3>& particle2_coords, double& pivot_rho, double& particle2_rho) const;
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
