//
// Created by bale2 on 26/09/2023.
//
#include "grid.hpp"
Constants calculate_constants(double ppm){
    double const h = smooth_length(ppm);
    double const nx = nx_calc(xmax, xmin, h);
    double const ny = ny_calc(ymax, ymin, h);
    double const nz = nz_calc(zmax, zmin, h);
    double const sx = sx_calc(xmax, xmin, nx);
    double const sy = sy_calc(ymax, ymin, ny);
    double const sz = sz_calc(zmax, zmin, nz);
    Constants constants(h, nx, ny, nz, sx, sy, sz);
    return constants;
}



double nx_calc (double x_max, double x_min, double h_param){
    return floor( (x_max - x_min) / h_param);
}
double ny_calc (double y_max, double y_min, double h_param){
    return floor( (y_max - y_min) / h_param);
}
double nz_calc (double z_max, double z_min, double h_param){
    return floor( (z_max - z_min) / h_param);
}

double smooth_length (double ppm){
    return static_cast<double>(radio) / ppm;
}

double sx_calc (double x_max, double x_min, double n_x){
    return (x_max-x_min)/n_x;
}

double sy_calc (double y_max, double y_min, double n_y){
    return (y_max-y_min)/n_y;
}

double sz_calc (double z_max, double z_min, double n_z){
    return (z_max-z_min)/n_z;
}

double particle_mass (double ppm){
    return static_cast<double>(rho_f)/(pow (ppm,3));
}

Malla create_fill_grid(double np,double ppm,double nz, double ny, double nx){
    int i, j, k;
    Malla malla(np, ppm, vector<Block>());
    // Create the blocks and append them to Malla. Create all vectors in nx, ny, nz
    for (k = 0; k < nz; ++k) {
        for (j = 0; j < ny; ++j) {
            for (i = 0; i < nx; ++i) {
                malla.blocks.push_back(Block(i, j, k, vector<Particle>(), check_neighbours(i, j, k, nx, ny, nz)));
            }
        }
    }
    return malla;
}

