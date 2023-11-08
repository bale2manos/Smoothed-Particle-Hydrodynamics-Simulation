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
double const rho_f = pow(10,3);
double const p_s = 3.0; //presión de rigidez
double const s_c = 3*10000;
double const d_v = 128.0;
double const mu = 0.4;
double const d_p = 2 * 0.0001;
double const delta_t = 0.001;
double const xmax = 0.065;
double const xmin = -0.065;
double const ymax = 0.1;
double const ymin = -0.08;
double const zmax = 0.065;
double const zmin = -0.065;
double const g = -9.8;


struct Malla {
    int np; double ppm; vector<Block> blocks;
    std::array<int,3>n_blocks;
    std::array<double,3> size_blocks;
    double h, m;

    std::array<double,2>acc_const;

};



void calculate_constants(double ppm, int np, Malla& malla);
int nx_calc (double xmax, double xmin, double h);
int ny_calc (double ymax, double ymin, double h);
int nz_calc (double zmax, double zmin, double h);
double sx_calc (double xmax, double xmin, int nx);
double sy_calc (double ymax, double ymin, int ny);
double sz_calc (double zmax, double zmin, int nz);
double smooth_length (double ppm);
double particle_mass (double ppm);
void create_fill_grid(Malla& malla, int np,double ppm);


void wall_colissions(Particle& particle, Block& block, array<int,3> n_blocks);
void edge_collisions(Particle& particula, int extremo, int eje);
void particle_movement(Particle& particle);
void limits_interaction(Particle& particle, Block& block, array<int,3> n_blocks);
void edge_interaction(Particle& particle,int extremo,int eje);


array<double,3> acceleration_transfer(Particle& pivot, Particle& particle2, double h, array<double,2> acc_const);
double increase_density(array<double, 3> pivot_coords, array<double, 3> particle2_coords, double h);
double density_transformation(double rho,double h, double m);

void malla_interaction (Malla& malla, vector<Particle>& particles);
void repos(Malla& malla, vector<Particle>& particles);
void densinc(Malla& malla, vector<Particle>& particles);
void denstransf(Malla& malla, vector<Particle>& particles);
void acctransf(Malla& malla, vector<Particle>& particles);
void triplete(Malla &malla, vector<Particle>& particles;
#endif //ARQUITECTURA_DE_COMPUTADORES_GRID_HPP
