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

Malla create_fill_grid(double np,double ppm,double nz, double ny, double nx, double h, double m,double sx, double sy, double sz){
    Malla malla(np, ppm, vector<Block>(),nx,ny,nz,h,m,sx,sy,sz);
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

Malla malla_interaction (Malla malla){
    vector<Particle> all_iterated_particles;

    for (Block & block : malla.blocks) {
        size_t const neighbours_size = block.neighbours.size();
        for (Particle & particle_pivot : block.particles) {
            Particle particle_updated = particle_pivot;
            for (size_t i = 0; i < neighbours_size; ++i) {
                for (Particle  const& particle2 : malla.blocks[i].particles) {
                    if (particle_updated.id == particle2.id) { continue; }
                    double const increase_d_factor = increase_density(particle_updated.p, particle2.p, malla.h);
                    particle_updated.rho = particle_updated.rho + increase_d_factor;
                    particle_updated.rho = density_transformation(particle_updated.rho, malla.h, malla.m);
                    array<double,3> new_acceleration = acceleration_transfer(particle_updated,particle2,malla.h,malla.m);
                    particle_updated.a = new_acceleration;

                }
            }
            bool block_edge = false;
            if (block.i == 0||block.j==0||block.k==0||block.i==malla.nx-1||block.j==malla.ny-1||block.k==malla.nz-1){
                block_edge = true;
            }

            if (block_edge){
                particle_updated = wall_colissions(particle_updated, block, malla.nx, malla.ny, malla.nz);
            }
            particle_pivot = particle_movement(particle_pivot);
            if (block_edge){
                limits_interaction(particle_updated, block, malla.nx, malla.ny, malla.nz);
            }
            all_iterated_particles.push_back(particle_updated);
        }
    }


    // Actualizar datos de las particulas
    for (Block & block : malla.blocks) {
       block.particles.clear();
    }
    for (Particle & particle: all_iterated_particles){
       int i,j,k;
       i = initial_block_index(particle.p[0], xmin,  malla.sx);
       j = initial_block_index(particle.p[1], ymin,  malla.sy);
       k = initial_block_index(particle.p[2], zmin,  malla.sz);
       /* TODO problema coma flotante floor error */
       if (i<0){i = 0;}
       if (j<0){j = 0;}
       if (k<0){k = 0;}
       if (i >= malla.nx) {i = malla.nx - 1;}
       if (j >= malla.ny){j = malla.ny - 1;}
       if (k >= malla.nz){k = malla.nz - 1;}
       int index = calculate_block_index(i,j,k,malla.nx,malla.ny);
       malla.blocks[index].particles.emplace_back(particle);
    }

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
    double first_term = (rho + pow(h,6));
    double second_term = (315/(64*M_PI*pow(h,9)));
    return first_term*second_term*m;
}


array<double,3> acceleration_transfer(Particle& pivot, Particle& particle2,double h,double m){
    array<double,3> acc_increase;
    //Aquí calculamos los términos por separado de la ecuación grande.
    double const norm = pow((pivot.p[0] - particle2.p[0]),2) + pow((pivot.p[1] - particle2.p[1]),2)
                        + pow((particle2.p[2] - particle2.p[2]),2);
    double const norm_squared = pow(norm,2);
    if (norm_squared>=pow(h,2)){return {0,0,0};}
    double const distij = sqrt(max(norm,pow(10,-12)));
    double const aux =M_PI*pow(h,6);
    double const const1 = 15/aux;
    double const const2 = (3*m*p_s)*0.5;
    double const const3 = const1*3; // Mejor multiplicar que dividir


    for (int i=0; i<3; i++){
        double const term1 = (pivot.p[i]-particle2.p[i]);
        double const term2 = pow(h-distij,2)/distij;
        double const term3 = (pivot.rho + particle2.rho - (2*rho_f));
        double const term4 = (particle2.v[i] - pivot.v[i]);
        double const denominator = (particle2.rho  * pivot.rho);
        acc_increase[i] = ((term1*const1*const2*term2*term3) + (term4*const3*mu*m))/denominator;
        pivot.a[i] = pivot.a[i] + acc_increase[i];
        }
  return pivot.a;
  }


Particle wall_colissions(Particle particle, Block block, int nx, int ny, int nz){

  if (block.i == 0 || block.i==nx-1){
        particle.a = edge_collisions(particle, block.i, 0);
  }
  if (block.j == 0||block.j == ny-1) {
        particle.a = edge_collisions(particle, block.j, 1);
  }
  if (block.k == 0 || block.k==nz-1){
        particle.a = edge_collisions(particle, block.k, 2);
  }
  return particle;
};


std::array<double,3> edge_collisions(Particle particula, int extremo, int eje) {
  double min_limit = NAN;
  double max_limit = NAN;
  if (eje == 0){ min_limit= xmin; max_limit = xmax;}
  if (eje == 1){ min_limit= ymin; max_limit = ymax;}
  if (eje == 2){ min_limit= zmin; max_limit = zmax;}

  double const coord = particula.p[eje] + particula.hv[eje] * delta_t;
  if (extremo == 0) {
        double deltax = d_p - (coord - min_limit);
        if (deltax > pow(10, -10)) {
            particula.a[eje] = particula.a[eje] + s_c * deltax - d_v * particula.v[eje];
        }
  } else {
        double deltax = d_p - (max_limit - coord);
        if (deltax > pow(10, -10)) {
            particula.a[eje] = particula.a[eje] - s_c * deltax + d_v * particula.v[eje];
        }
  }
  return particula.a;
}


Particle particle_movement(Particle particle){
    for(int i =0;i<3;i++){
        particle.p[i] = particle.p[i] + (particle.hv[i]*delta_t) + (particle.a[i]*pow(delta_t,2));
        particle.v[i] = particle.hv[i]+(particle.a[i]*delta_t*0.5);
        particle.hv[i] = particle.hv[i]+(particle.a[i]*delta_t);
    }
    return particle;
}

Particle limits_interaction(Particle particle, Block block, int nx, int ny, int nz){

    if (block.i == 0 || block.i==nx-1){
        particle = edge_interaction(particle, block.i, 0);
    }
    if (block.j == 0||block.j == ny-1) {
        particle = edge_interaction(particle, block.j, 1);
    }
    if (block.k == 0 || block.k==nz-1){
        particle = edge_interaction(particle, block.k, 2);
    }
    return particle;

};

void edge_interaction(Particle& particle,int extremo,int eje){
    double min_limit = NAN;
    double max_limit = NAN;
    if (eje == 0){ min_limit= xmin; max_limit = xmax;}
    if (eje == 1){ min_limit= ymin; max_limit = ymax;}
    if (eje == 2){ min_limit= zmin; max_limit = zmax;}
    double displacement_edge = 0.0;
    if (extremo == 0){
      displacement_edge = particle.p[eje] - min_limit;
      particle.p[eje] = min_limit - displacement_edge;
    }
    else{
      displacement_edge = max_limit - particle.p[eje];
      particle.p[eje]= max_limit + displacement_edge;
    }
    for (int i = 0; i<3;i++){
      particle.v[i] = -particle.v[i];
      particle.hv[i] = -particle.hv[i];
    }
}



