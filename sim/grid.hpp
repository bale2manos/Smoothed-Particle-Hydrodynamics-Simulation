//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_GRID_HPP
#define ARQUITECTURA_DE_COMPUTADORES_GRID_HPP

#include <vector>
#include "block.hpp"
#include <cmath>


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

struct Constants{
    double h, nx, ny, nz, sx, sy, sz;
};

struct Malla {
    int np; float ppm; vector<Block> blocks;
};

Constants calculate_constants(double ppm);
double nx_calc (double xmax, double xmin, double h);
double ny_calc (double ymax, double ymin, double h);
double nz_calc (double zmax, double zmin, double h);
double sx_calc (double xmax, double xmin, double nx);
double sy_calc (double ymax, double ymin, double ny);
double sz_calc (double zmax, double zmin, double nz);
double smooth_length (double ppm);
double particle_mass (double ppm);
Malla create_fill_grid(double np,double ppm,double nz, double ny, double nx);
#endif //ARQUITECTURA_DE_COMPUTADORES_GRID_HPP
