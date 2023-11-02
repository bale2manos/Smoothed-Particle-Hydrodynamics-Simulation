//
// Created by bale2 on 26/09/2023.
//
#include "grid.hpp"
#include "progargs.hpp"
# define M_PI           3.14159265358979323846  /* pi */
#include <math.h>
#include <unordered_map>
#include <iostream>
void calculate_constants(double ppm, int np, Malla& malla){
    malla.ppm = ppm;
    malla.np = np;
    malla.h = smooth_length(ppm);
    malla.m = particle_mass(ppm);
    malla.n_blocks[0] = nx_calc(xmax, xmin, malla.h);
    malla.n_blocks[1] = ny_calc(ymax, ymin, malla.h);
    malla.n_blocks[2] = nz_calc(zmax, zmin, malla.h);
    malla.size_blocks[0] = sx_calc(xmax, xmin, malla.n_blocks[0]);
    malla.size_blocks[1] = sy_calc(ymax, ymin, malla.n_blocks[1]);
    malla.size_blocks[2] = sz_calc(zmax, zmin, malla.n_blocks[2]);
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

double sx_calc (double x_max, double x_min, int n_x){
    return (x_max-x_min)/n_x;
}

double sy_calc (double y_max, double y_min, int n_y){
    return (y_max-y_min)/n_y;
}

double sz_calc (double z_max, double z_min, int n_z){
    return (z_max-z_min)/n_z;
}

double particle_mass (double ppm){
    return static_cast<double>(rho_f)/(pow (ppm,3));
}

void create_fill_grid(Malla& malla, int np,double ppm){
    //Malla malla(np, ppm, vector<Block>(),nx,ny,nz,h,m,sx,sy,sz);
    calculate_constants(ppm, np, malla);
    malla.blocks = {};
        // Create the blocks and append them to Malla. Create all vectors in nx, ny, nz
    for (int k = 0; k < malla.n_blocks[2]; ++k) {
        for (int j = 0; j < malla.n_blocks[1]; ++j) {
            for (int i = 0; i < malla.n_blocks[0]; ++i) {
                std::array<int, 3> const indexes = {i,j,k};
                malla.blocks.push_back(Block(indexes, {}, check_neighbours({i,j,k}, malla.n_blocks)));
            }
        }
    }
}

void malla_interaction_old (Malla& malla){
    vector<Particle> all_iterated_particles;
    for (Block & block : malla.blocks) {
        size_t const neighbours_size = block.neighbours.size();
        for (Particle & particle_pivot : block.particles) {
            Particle particle_updated = particle_pivot;
            for (size_t i = 0; i < neighbours_size; ++i) {
                for (Particle& particle2 : malla.blocks[i].particles) {
                  if (particle_updated.id == particle2.id) { continue; }
                  double const increase_d_factor = increase_density(particle_updated.p, particle2.p, malla.h);
                  particle_updated.rho = particle_updated.rho + increase_d_factor;
                  particle_updated.rho = density_transformation(particle_updated.rho, malla.h, malla.m);
                  array<double,3> new_acceleration = acceleration_transfer(particle_updated,particle2,malla.h,malla.m);
                  particle_updated.a = new_acceleration;


                }
            }
            bool block_edge = false;
            if (block.coords[0] == 0||block.coords[1]==0||block.coords[2]==0||
                block.coords[0]==malla.n_blocks[0]-1||block.coords[1]==malla.n_blocks[1]-1||block.coords[2]==malla.n_blocks[2]-1){
                block_edge = true;
            }

            if (block_edge){
                wall_colissions(particle_updated, block, malla.n_blocks);
            }
            particle_movement(particle_pivot);
            if (block_edge){
                limits_interaction(particle_updated, block, malla.n_blocks);
            }
            all_iterated_particles.push_back(particle_updated);
        }
    }


    // Actualizar datos de las particulas
    update_grid(malla, all_iterated_particles);

}

double increase_density(array<double, 3> pivot_coords, array<double, 3> particle2_coords, double h){
    double const norm_squared = pow((pivot_coords[0] - particle2_coords[0]),2) + pow((pivot_coords[1] - particle2_coords[1]),2)
            + pow((pivot_coords[2] - particle2_coords[2]),2);
    double const h_squared = pow(h,2);
    if (norm_squared < h_squared){
        return pow(h_squared-norm_squared,3);
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
    double const norm_squared = pow((pivot.p[0] - particle2.p[0]),2) + pow((pivot.p[1] - particle2.p[1]),2)
                        + pow((pivot.p[2] - particle2.p[2]),2);

    if (norm_squared>=pow(h,2)){return {0,0,0};}
    double const distij = sqrt(max(norm_squared,pow(10,-12)));
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
        //Hacer return solo del increase
        //pivot.a[i] = pivot.a[i] + acc_increase[i];
        }
  //return pivot.a;
    return acc_increase;
  }

/*TODO ampersand block? */
void wall_colissions(Particle& particle, Block& block, array<int,3>n_blocks){

  if (block.coords[0] == 0 || block.coords[0]==n_blocks[0]-1){
        edge_collisions(particle, block.coords[0], 0);
  }
  if (block.coords[1] == 0||block.coords[1] == n_blocks[1]-1) {
        edge_collisions(particle, block.coords[1], 1);
  }
  if (block.coords[2] == 0 || block.coords[2]==n_blocks[2]-1){
        edge_collisions(particle, block.coords[2], 2);
  }
};


void edge_collisions(Particle& particula, int extremo, int eje) {
  double min_limit = NAN;
  double max_limit = NAN;
  if (eje == 0){ min_limit= xmin; max_limit = xmax;}
  if (eje == 1){ min_limit= ymin; max_limit = ymax;}
  if (eje == 2){ min_limit= zmin; max_limit = zmax;}

  double const coord = particula.p[eje] + particula.hv[eje] * delta_t;
  if (extremo == 0) {
        double deltax = d_p - (coord - min_limit);
        if (deltax > pow(10, -10)) {
            particula.a[eje] = particula.a[eje] + (s_c * deltax - d_v * particula.v[eje]);
        }
  } else {
        double deltax = d_p - (max_limit - coord);
        if (deltax > pow(10, -10)) {
            particula.a[eje] = particula.a[eje] - (s_c * deltax + d_v * particula.v[eje]);
        }
  }
}


void particle_movement(Particle& particle){
    for(int i =0;i<3;i++){
        particle.p[i] = particle.p[i] + (particle.hv[i]*delta_t) + (particle.a[i]*pow(delta_t,2));
        particle.v[i] = particle.hv[i]+(particle.a[i]*delta_t*0.5);
        particle.hv[i] = particle.hv[i]+(particle.a[i]*delta_t);
    }
}

void limits_interaction(Particle& particle, Block& block, array<int,3>n_blocks) {
    if (block.coords[0] == 0 || block.coords[0] == n_blocks[0] - 1) { edge_interaction(particle, block.coords[0], 0); }
    if (block.coords[1] == 0 || block.coords[1] == n_blocks[1] - 1) { edge_interaction(particle, block.coords[1], 1); }
    if (block.coords[2] == 0 || block.coords[2] == n_blocks[2] - 1) { edge_interaction(particle, block.coords[2], 2); }
}

void edge_interaction(Particle& particle,int extremo,int eje){
    double min_limit = 0;
    double max_limit = 0;
    if (eje == 0){ min_limit= xmin; max_limit = xmax;}
    if (eje == 1){ min_limit= ymin; max_limit = ymax;}
    if (eje == 2){ min_limit= zmin; max_limit = zmax;}
    double displacement_edge = 0.0;
    if (extremo == 0){
      displacement_edge = particle.p[eje] - min_limit;
      if(displacement_edge<0) {
            particle.p[eje] = min_limit - displacement_edge;
      }
    }
    else{
      displacement_edge = max_limit - particle.p[eje];
      if(displacement_edge<0) {
            particle.p[eje] = max_limit + displacement_edge;
      }
    }
    if(displacement_edge<0) {
        particle.v[eje]  *= -1;
        particle.hv[eje] *= -1;
    }
}





void update_grid(Malla& malla, vector<Particle>& new_particles){
    for (Block & block : malla.blocks) {
      block.particles.clear();
    }
    for (Particle  const& particle: new_particles){
      malla.blocks[particle.current_block].particles.emplace_back(particle);
    }
}

void densinc_old(Malla& malla){
    vector<Particle> all_iterated_particles;
    for (Block & block : malla.blocks) {
      for (Particle & particle_pivot : block.particles) {
            Particle particle_updated = particle_pivot;
            for (auto index: block.neighbours) {
                for (Particle & particle2 : malla.blocks[index].particles) {
                    if (particle_updated.id == particle2.id) { continue; }
                    double const increase_d_factor =
                        increase_density(particle_updated.p, particle2.p, malla.h);
                    particle_updated.rho = particle_updated.rho + increase_d_factor;
                }
            }
            all_iterated_particles.push_back(particle_updated);
      }
    }
    update_grid(malla, all_iterated_particles);
}

void densinc(Malla& malla){
    //vector<Particle> all_iterated_particles;
    unordered_map<int, vector<int>> colisiones_map;
    vector<double> new_densities(malla.np, 0);


    for (Block & block : malla.blocks) {
      for (Particle & particle_pivot : block.particles) {
            vector<int> colisiones_pivote = colisiones_map[particle_pivot.id];
            //Particle particle_updated = particle_pivot
            for (auto index: block.neighbours) {
                for (Particle & particle2 : malla.blocks[index].particles) {
                    // TODO esto optimiza? // TODO extraer metodo de check colisions?
                    // int particle1_id = particle_updated.id; particle2_id = particle2.id;
                    if (particle_pivot.id == particle2.id
                        || already_iterated(colisiones_pivote, particle2.id)) {
                      continue;}
                    colisiones_map[particle_pivot.id].push_back(particle2.id);
                    colisiones_map[particle2.id].push_back(particle_pivot.id);
                    double const increase_d_factor =
                        increase_density(particle_pivot.p, particle2.p, malla.h);
                    new_densities[particle_pivot.id] += increase_d_factor;
                    new_densities[particle2.id] += increase_d_factor;
                }
            }
            //all_iterated_particles.push_back(particle_updated);
      }
    }

    for (Block & block : malla.blocks) {
      for (Particle & particle : block.particles) { particle.rho = new_densities[particle.id]; }
    }
    //update_grid(malla, all_iterated_particles);
}


bool already_iterated(vector<int> &pivot_list, int particle2_id){

  for (const int& valor : pivot_list) {
    if (valor == particle2_id) {
          cout<<"ENTRO: ID PIVOTE: "<< valor<<"ID 2: "<<"\n";
          return true;
    }
  }
  return false;
}



void denstransf(Malla& malla){
    for (Block & block : malla.blocks) {
      for (Particle & particle_pivot : block.particles) {
            particle_pivot.rho = density_transformation(particle_pivot.rho, malla.h, malla.m);
      }
    }
}

void acctransf_old(Malla& malla){
    vector<Particle> all_iterated_particles;
    for (Block & block : malla.blocks) {
      for (Particle & particle_pivot : block.particles) {
            Particle particle_updated = particle_pivot;
            for (auto index: block.neighbours) {
                for (Particle & particle2 : malla.blocks[index].particles) {
                    if (particle_updated.id == particle2.id) { continue; }
                    array<double,3> acc_incr = acceleration_transfer(particle_updated,particle2,malla.h,malla.m);
                    for(int i=0;i<3;i++) { particle_updated.a[i] += acc_incr[i]; }
                }
            }
            all_iterated_particles.push_back(particle_updated);
      }
    }
    update_grid(malla, all_iterated_particles);
}

void acctransf(Malla& malla){
    //vector<Particle> all_iterated_particles;
    unordered_map<int, vector<int>> colisiones_map;
    vector<Acceleration> new_accelerations(malla.np, {0,g,0});
    for (Block & block : malla.blocks) {
      for (Particle & particle_pivot : block.particles) {
            vector<int> colisiones_pivote = colisiones_map[particle_pivot.id];
            //Particle particle_updated = particle_pivot;
            for (auto index: block.neighbours) {
                for (Particle & particle2 : malla.blocks[index].particles) {
                    if (particle_pivot.id == particle2.id
                        || already_iterated(colisiones_pivote, particle2.id)) {
                      continue;}
                    colisiones_map[particle_pivot.id].push_back(particle2.id);
                    colisiones_map[particle2.id].push_back(particle_pivot.id);
                    array<double,3> acc_incr = acceleration_transfer(particle_pivot,particle2,malla.h,malla.m);
                    new_accelerations[particle_pivot.id].accx += acc_incr[0];
                    new_accelerations[particle_pivot.id].accy += acc_incr[1];
                    new_accelerations[particle_pivot.id].accz += acc_incr[2];
                    new_accelerations[particle2.id].accx -= acc_incr[0];
                    new_accelerations[particle2.id].accy -= acc_incr[1];
                    new_accelerations[particle2.id].accz -= acc_incr[2];
                }
            }
            //all_iterated_particles.push_back(particle_updated);
      }
    }
    for (Block & block : malla.blocks) {
      for (Particle & particle : block.particles) {
            // TODO creo que PARTICLE puede ser un struct Acceleration
            particle.a[0] = new_accelerations[particle.id].accx;
            particle.a[1] = new_accelerations[particle.id].accy;
            particle.a[2] = new_accelerations[particle.id].accz;
      }
    }
    //update_grid(malla, all_iterated_particles);
}

void partcol(Malla& malla){
    for (Block & block : malla.blocks) {
      for (Particle & particle_pivot : block.particles) {
            if (block.coords[0] == 0||block.coords[1]==0||block.coords[2]==0||
                block.coords[0]==malla.n_blocks[0]-1||block.coords[1]==malla.n_blocks[1]-1||block.coords[2]==malla.n_blocks[2]-1){
                wall_colissions(particle_pivot, block, malla.n_blocks);
            }
      }
    }
}

void motion(Malla& malla){
    for (Block & block : malla.blocks) {
      for (Particle & particle_pivot : block.particles) {
            particle_movement(particle_pivot);
      }
    }
}

void boundint(Malla& malla){
    for (Block & block : malla.blocks) {
      for (Particle & particle_pivot : block.particles) {
            if (block.coords[0] == 0||block.coords[1]==0||block.coords[2]==0||
                block.coords[0]==malla.n_blocks[0]-1||block.coords[1]==malla.n_blocks[1]-1||block.coords[2]==malla.n_blocks[2]-1){
                limits_interaction(particle_pivot, block, malla.n_blocks);
            }
      }
    }
}

void repos_old(Malla& malla){
    // Iterar por todos los bloques de la malla
    for (size_t current_block= 0; current_block < malla.blocks.size(); current_block++) {
      // Iterar por todas las particulas del bloque
      for (size_t current_part= 0; current_part < malla.blocks[current_block].particles.size(); current_part++){
            // Calcular el nuevo posible bloque de la particula
            array<int,3> const new_indexes =
                calculate_block_indexes(malla.blocks[current_block].particles[current_part].p, malla);
            int const block_index = calculate_block_index(new_indexes,malla.n_blocks[0],malla.n_blocks[1]);
            auto new_block = static_cast<size_t>(block_index);
            int const part_old = static_cast<int>(current_part);

            // Si el bloque es distinto al actual, mover la particula
            if (new_block != current_block){
                if (malla.blocks[current_block].particles[part_old].id == 0){
                    cout << "NEW: " << new_block << "OLD: " << current_block<< "\n";
                }
                malla.blocks[new_block].particles.push_back(malla.blocks[current_block].particles[part_old]);
                malla.blocks[current_block].particles.erase(malla.blocks[current_block].particles.begin() + part_old);
            }
      }
    }
}

void initacc(Malla& malla){
    for (Block & block : malla.blocks) {
      for (Particle & particle_pivot : block.particles) {
            particle_pivot.a = {0, g, 0};
            particle_pivot.rho = 0;
      }
    }
}

void repos(Malla& malla){
    vector<Particle> particles;
    for (Block & block : malla.blocks) {
      for (Particle & particle_pivot : block.particles) {
            particles.push_back(particle_pivot);
      }
      block.particles.clear();
    }

    for (Particle & particle : particles) {
      array<int,3> const new_indexes =
              calculate_block_indexes(particle.p, malla);
      int const block_index = calculate_block_index(new_indexes,malla.n_blocks[0],malla.n_blocks[1]);
      particle.current_block = block_index;
      auto new_block = static_cast<size_t>(block_index);
      malla.blocks[new_block].particles.push_back(particle);
    }


}



void malla_interaction(Malla& malla){
    repos(malla);
    initacc(malla);
    densinc(malla);
    denstransf(malla);
    acctransf(malla);
    partcol(malla);
    motion(malla);
    boundint(malla);
}