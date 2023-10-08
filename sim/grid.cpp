//
// Created by bale2 on 26/09/2023.
//
#include "grid.hpp"
#include <math.h>
#include <iostream>
Constants calculate_constants(double ppm){
    double const h = smooth_length(ppm);
    double const m = particle_mass(ppm);
    int const nx = nx_calc(xmax, xmin, h);
    int const ny = ny_calc(ymax, ymin, h);
    int const nz = nz_calc(zmax, zmin, h);
    double const sx = sx_calc(xmax, xmin, nx);
    double const sy = sy_calc(ymax, ymin, ny);
    double const sz = sz_calc(zmax, zmin, nz);
    Constants constants(nx, ny, nz, h, m, sx, sy, sz);
    return constants;
}



int nx_calc (double x_max, double x_min, double h_param){
    return floor( (x_max - x_min) / h_param);
}
int ny_calc (double y_max, double y_min, double h_param){
    return floor( (y_max - y_min) / h_param);
}
int nz_calc (double z_max, double z_min, double h_param){
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

Malla create_fill_grid(double np,double ppm,double nz, double ny, double nx, double h, double m){
    Malla malla(np, ppm, vector<Block>(),nx,ny,nz,h,m);
    // Create the blocks and append them to Malla. Create all vectors in nx, ny, nz
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                malla.blocks.push_back(Block(i, j, k, vector<Particle>(), check_neighbours(i, j, k, nx, ny, nz)));
            }
        }
    }
    return malla;
}

Malla cuatropunto3punto2 (Malla malla){
    for (Block & block : malla.blocks) {
        size_t const neighbours_size = block.neighbours.size();
        for (Particle & particle_pivot : block.particles) {
            for (size_t i = 0; i < neighbours_size; ++i) {
                for (Particle  const& particle2 : malla.blocks[i].particles) {
                    double const increase_d_factor = increase_density(particle_pivot.p, particle2.p, malla.h);
                    particle_pivot.rho = particle_pivot.rho + increase_d_factor;
                    particle_pivot.rho = density_transformation(particle_pivot.rho, malla.h, malla.m);
                }
            }
        }
    }

    return malla;
}




double increase_density(array<double, 3> pivot_coords, array<double, 3> particle2_coords, double h){
    double const norm = pow((pivot_coords[0] - particle2_coords[0]),2) + pow((pivot_coords[1] - particle2_coords[1]),2)
            + pow((pivot_coords[2] - particle2_coords[2]),2);
    double const h_squared = pow(h,2);
    if (norm < h_squared){
        return pow(h_squared-norm,3);
    }
    return 0;
}



double density_transformation(double rho,double h, double m){
    double const first_term = (rho + pow(h,6));
    double const second_term = (315/(64*M_PI*pow(h,9)));
    return first_term*second_term*m;
}


double calcdistij(double const mod1, double const mod2){
    double param = mod1-mod2;
    double const compare = sqrt(1e-12);
    if (param < compare){
        param = compare;
    }
    double const distij = sqrt(param);
    return distij;
}

int increase_accel(Particle Particle1, Particle Particle2, double h, double m){
    double const mod1 = sqrt(pow(Particle1.p[0], 2) + pow(Particle1.p[1], 2)+ pow(Particle1.p[2], 2));
    double const mod2 = sqrt(pow(Particle2.p[0], 2) + pow(Particle2.p[1], 2)+ pow(Particle2.p[2], 2));
    double const distij = calcdistij(mod1, mod2);
    double const calc1 = 15/(M_PI*pow(h,6));
    double const calc2 = 15/(M_PI* pow(h,6));
    double const calc3 = pow((h-distij),2)/distij;
    if (pow((mod1 - mod2), 2) < pow(h,2)){

        for (int i = 0; i<3; i++) {
            double const diffij = ((Particle1.p[i] - Particle2.p[i])*calc1*m*calc3*(Particle1.rho + Particle2.rho - 2*rho_f) +
                                   (Particle1.v[i]- Particle2.v[i])*calc2*mu*m);
            Particle1.a[i] = Particle1.a[i] + diffij;

        }
    }


    return 0;
}





Malla changeaccel ( Malla malla){
    for (Block  const& block : malla.blocks) {
        size_t const neighbours_size = block.neighbours.size();
        for (Particle  const& particle_pivot : block.particles) {
            for (size_t i = 0; i < neighbours_size; ++i) {
                for (Particle  const& particle2 : malla.blocks[i].particles) {
                    increase_accel(particle_pivot, particle2, malla.h, malla.m);
                }
            }
        }
    }
    return malla;
}



Particle particle_calc(Particle particle){
    for (int i = 0; i<3; i++) {
        particle.p[i] = particle.p[i] + (particle.hv[i]*delta_t) + (particle.a[i]*delta_t);
        particle.v[i] = particle.hv[i] + ((particle.a[i]*delta_t)/2);
        particle.hv[i] = particle.hv[i] + particle.a[i]*delta_t;
    }
    return particle;
}



Malla updateparticle(Malla malla){
    for (Block  const& block : malla.blocks) {
        for (Particle  const& particle_pivot : block.particles) {
            particle_calc(particle_pivot);
        }
    }
    return malla;
}






