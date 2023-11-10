//
// Created by bale2 on 26/09/2023.
//
#include "grid.hpp"

#include <math.h>
#include "progargs.hpp"
# define M_PI           3.14159265358979323846  /* pi */
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <chrono>

Malla::Malla(int np, double ppm) :
    np(np)
    , ppm(ppm), h(smooth_length()), m(particle_mass())

  {
  nBlocks = {nx_calc(xmax, xmin, h), ny_calc(ymax, ymin, h), nz_calc(zmax, zmin, h)};
  sizeBlocks = {sx_calc(xmax, xmin, nBlocks[0]), sy_calc(ymax, ymin, nBlocks[1]), sz_calc(zmax, zmin, nBlocks[2])};
  // TODO hacer esto en una funcion o mas bonito
  accConst = {15/(M_PI*pow(h,6)) * (3*m*p_s)*0.5, 15/(M_PI*pow(h,6))*3*mu*m};
  blocks = createFillGrid();
  particles = vector<Particle>(np);
}

std::vector<Particle> Malla::getParticles() const {
  return particles;
}

double Malla::getH() const {
  return h;
}

int Malla::getNp() const {
  return np;
}

double Malla::getPpm() const {
  return ppm;
}

std::array<int, 3> Malla::getNBlocks() const {
  return nBlocks;
}

std::array<double, 3> Malla::getSizeBlocks() const {
  return sizeBlocks;
}


array<int, 3> Malla::calculate_block_indexes(array <double,3> positions){
  int i_index=0;
  int j_index=0;
  int k_index=0;
  i_index = initial_block_index(positions[0], xmin,  sizeBlocks[0]);
  j_index = initial_block_index(positions[1], ymin,  sizeBlocks[1]);
  k_index = initial_block_index(positions[2], zmin,  sizeBlocks[2]);
  /* TODO problema coma flotante floor error */
  if (i_index<0){i_index = 0;}
  if (j_index<0){j_index = 0;}
  if (k_index<0){k_index = 0;}
  if (i_index >= nBlocks[0]) {i_index = nBlocks[0] - 1;}
  if (j_index >= nBlocks[1]){j_index = nBlocks[1] - 1;}
  if (k_index >= nBlocks[2]){k_index = nBlocks[2] - 1;}
  return array<int, 3>{i_index,j_index,k_index};
}

void Malla::insert_particles (const char * in_file) {
  ifstream input_file(in_file, ios::binary);
  double trash;
  input_file.read(reinterpret_cast<char *>(&trash), sizeof(double));
  array<array<float, 3>, 3> info_particle;
  array<array<double, 3>, 3> info_particle_double;
  int counter = 0;
  while (input_file.read(reinterpret_cast<char *>(&info_particle[0][0]), sizeof(info_particle[0][0]))) {
    // if i < np then read the next 8 floats, else continue
    input_file.read(reinterpret_cast<char *>(&info_particle[0][1]), sizeof(info_particle[0][1]));
    input_file.read(reinterpret_cast<char *>(&info_particle[0][2]), sizeof(info_particle[0][2]));
    input_file.read(reinterpret_cast<char *>(&info_particle[1][0]), sizeof(info_particle[1][0]));
    input_file.read(reinterpret_cast<char *>(&info_particle[1][1]), sizeof(info_particle[1][1]));
    input_file.read(reinterpret_cast<char *>(&info_particle[1][2]), sizeof(info_particle[1][2]));
    input_file.read(reinterpret_cast<char *>(&info_particle[2][0]), sizeof(info_particle[2][0]));
    input_file.read(reinterpret_cast<char *>(&info_particle[2][1]), sizeof(info_particle[2][1]));
    input_file.read(reinterpret_cast<char *>(&info_particle[2][2]), sizeof(info_particle[2][2]));
    for (size_t i = 0; i < info_particle.size(); ++i) {
      for (size_t j = 0; j < info_particle[i].size(); ++j) {
        info_particle_double[i][j] = static_cast<double>(info_particle[i][j]);
      }
    }
    array<int, 3> index_array = calculate_block_indexes(info_particle_double[0]);
    // Linear mapping para encontrar el bloque correcto
    int index = index_array[0] + index_array[1] * nBlocks[0] + index_array[2] * nBlocks[0] * nBlocks[1];
    insert_particle_info(info_particle_double,counter, index);
    counter++;
  }
  check_missmatch_particles(counter, np);
}

void Malla::insert_particle_info(array<array<double, 3>, 3> info, int id, int block_index){
  Particle particle{};
  particle.p = info[0];
  particle.hv = info[1];
  particle.v = info[2];
  particle.a = {0,0,0};
  particle.rho = 0;
  particle.id = id;
  particle.current_block = block_index;
  blocks[block_index].particles.push_back(id);
  particles[id] = particle;
}

int Malla::nx_calc (double x_max, double x_min, double h_param){
    return floor( (x_max - x_min) / h_param);
}
int Malla::ny_calc (double y_max, double y_min, double h_param){
    return floor( (y_max - y_min) / h_param);
}
int Malla::nz_calc (double z_max, double z_min, double h_param){
    return floor( (z_max - z_min) / h_param);
}

double Malla::smooth_length (){
    return static_cast<double>(radio) / ppm;
}

double Malla::sx_calc (double x_max, double x_min, int n_x){
    return (x_max-x_min)/n_x;
}

double Malla::sy_calc (double y_max, double y_min, int n_y){
    return (y_max-y_min)/n_y;
}

double Malla::sz_calc (double z_max, double z_min, int n_z){
    return (z_max-z_min)/n_z;
}

double Malla::particle_mass (){
    return static_cast<double>(rho_f)/(pow (ppm,3));
}

vector<Block> Malla::createFillGrid(){
    // Create the blocks and append them to Malla. Create all vectors in nx, ny, nz
    for (int k = 0; k < nBlocks[2]; ++k) {
        for (int j = 0; j < nBlocks[1]; ++j) {
            for (int i = 0; i < nBlocks[0]; ++i) {
                std::array<int, 3> const indexes = {i,j,k};
                blocks.emplace_back(indexes, nBlocks);
            }
        }
    }
    return blocks;
}


void Malla::increase_density(array<double, 3>& pivot_coords, array<double, 3>& particle2_coords, double& pivot_rho, double& particle2_rho) {
    const static auto h_squared = h * h;
    double const dx_differ = pivot_coords[0] - particle2_coords[0];
    double const dy_differ = pivot_coords[1] - particle2_coords[1];
    double const dz_differ = pivot_coords[2] - particle2_coords[2];
    double const norm_squared = dx_differ * dx_differ + dy_differ * dy_differ + dz_differ * dz_differ;
    if (norm_squared < h_squared) {
        double const diff = (h_squared - norm_squared);
        double const increase =  diff *  diff * diff;
        pivot_rho += increase;
        particle2_rho += increase;
    }
}


double Malla::density_transformation(double rho){
    const static auto h_sixth = pow(h,6);
    const static auto h_ninth = pow(h,9);
    double const first_term = (rho + h_sixth);
    const static auto second_term = (315/(64*M_PI*h_ninth));
    return first_term*second_term*m;
}

void Malla::acceleration_transfer(Particle & pivot, Particle & particle2) {
    array<double,3>& position_pivot = pivot.p;
    array<double,3>& position_2 = particle2.p;


    double const differ_x = position_pivot[0] - position_2[0];
    double const differ_y = position_pivot[1] - position_2[1];
    double const differ_z = position_pivot[2] - position_2[2];

    double const norm = sqrt(
        differ_x * differ_x + differ_y * differ_y + differ_z * differ_z
    );

    if (norm <= h) {
        const static auto factor    = 1e-6;
        const static auto doble_rho = 2 * rho_f;
        double const distij         = max(norm, factor);
        double const difference_h_distij = h - distij;

        double const term2 = difference_h_distij*difference_h_distij / distij;
        // Sacamos todas las constantes fuera del bucle.
        double const density_pivot = pivot.rho;
        double const density_2     = particle2.rho;
        double const term3         = (density_pivot + density_2 - (doble_rho));
        double const denominator   = 1 / (density_2 * density_pivot);
        double const numerator1    = accConst[0] * term2 * term3;

        for (int i = 0; i < 3; i++) {
            double const term1  = (pivot.p[i] - particle2.p[i]);
            double const term4  = (particle2.v[i] - pivot.v[i]);
            pivot.a[i]         += ((term1 * numerator1) + (term4 * accConst[1])) * denominator;
            particle2.a[i]     -= ((term1 * numerator1) + (term4 * accConst[1])) * denominator;
        }
    }
}


void Malla::wall_colissions(Particle& particle, Block& block){

  if (block.coords[0] == 0 || block.coords[0]==nBlocks[0]-1){
        edge_collisions(particle, block.coords[0], 0);
  }
  if (block.coords[1] == 0||block.coords[1] == nBlocks[1]-1) {
        edge_collisions(particle, block.coords[1], 1);
  }
  if (block.coords[2] == 0 || block.coords[2]==nBlocks[2]-1){
        edge_collisions(particle, block.coords[2], 2);
  }
}


void Malla::edge_collisions(Particle& particula, int extremo, int eje) {
  double min_limit = 0.0;
  double max_limit = 0.0;
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


void Malla::particle_movement(Particle& particle){

    double delta_t_squared = delta_t * delta_t;
    double delta_t_half = delta_t*0.5;

    for(int i =0;i<3;i++){
        double hv_i = particle.hv[i];
        double acc_i = particle.a[i];

        particle.p[i] += (hv_i*delta_t) + (acc_i*delta_t_squared);
        particle.v[i] = hv_i + (acc_i*delta_t_half);
        particle.hv[i] += (acc_i*delta_t);
    }
}




void Malla::limits_interaction(Particle& particle, Block& block) {
    if (block.coords[0] == 0 || block.coords[0] == nBlocks[0] - 1) { edge_interaction(particle, block.coords[0], 0); }
    if (block.coords[1] == 0 || block.coords[1] == nBlocks[1] - 1) { edge_interaction(particle, block.coords[1], 1); }
    if (block.coords[2] == 0 || block.coords[2] == nBlocks[2] - 1) { edge_interaction(particle, block.coords[2], 2); }
}

void Malla::edge_interaction(Particle& particle,int extremo,int eje){
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


void Malla::densinc(){

    for (Particle & particle_pivot: particles){
        int const particle_pivot_id = particle_pivot.id;
        Block & current_block =  blocks[particle_pivot.current_block];
        vector<int>& neighbours_particles = current_block.neighbours_particles;
        for (auto particle2_id: neighbours_particles){
            if(particle_pivot_id < particle2_id){
                Particle & particle2 = particles[particle2_id];
                increase_density(particle_pivot.p, particle2.p, particle_pivot.rho, particle2.rho);
            }
        }
        particle_pivot.rho = density_transformation(particle_pivot.rho);
    }

}


void Malla::acctransf(){

    for (Particle & particle_pivot: particles){
        int const particle_pivot_id = particle_pivot.id;
        Block const & current_block =  blocks[particle_pivot.current_block];
        vector<int> const & neighbours_particles = current_block.neighbours_particles;
        for (auto particle2_id: neighbours_particles){
           if(particle_pivot_id < particle2_id){
               Particle & particle2 = particles[particle2_id];
               acceleration_transfer(particle_pivot, particle2);
           }
       }
    }
}




void Malla::triplete(){
    array<int,3> const n_blocks = nBlocks;
    for (Particle & particle_pivot : particles) {
        Block& block = blocks[particle_pivot.current_block];
        bool extremo = false;

        if (block.coords[0] == 0||block.coords[1]==0||block.coords[2]==0||
            block.coords[0]==n_blocks[0]-1||block.coords[1]==n_blocks[1]-1||block.coords[2]==n_blocks[2]-1){
            extremo = true;
            wall_colissions(particle_pivot, block);
        }
        particle_movement(particle_pivot);
        if (extremo){
            limits_interaction(particle_pivot, block);
        }
    }
}



void Malla::repos(){
    for (Block & block : blocks) {
      block.particles.clear();
      block.neighbours_particles.clear();
    }

    for (Particle & particle : particles) {
      array<int,3> const new_indexes =
              calculate_block_indexes(particle.p);
      int const block_index = calculate_block_index(new_indexes, nBlocks[0], nBlocks[1]);
      particle.current_block = block_index;

      // antiguo initacc
      particle.rho = 0;
      particle.a = {0, g, 0};
      // fin antiguo initacc

      auto new_block = static_cast<size_t>(block_index);
      blocks[new_block].particles.emplace_back(particle.id);
    }

    int currentBlock = 0;
    for (Block & block: blocks){
        for (auto index: block.neighbours){
            Block& neighbour_block = blocks[index];
            for (auto particle_id: neighbour_block.particles){
                blocks[currentBlock].neighbours_particles.emplace_back(particle_id);
            }
        }
        currentBlock++;
    }


}


void Malla::mallaInteraction(){
    auto startTimeTotal = std::chrono::high_resolution_clock::now();

    auto startTime = std::chrono::high_resolution_clock::now();
    Malla::repos();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "TIEMPO repos: " << duration.count() << " microsegundos" << "\n";

    startTime = std::chrono::high_resolution_clock::now();
    Malla::densinc();
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "TIEMPO densinc: " << duration.count() << " microsegundos" << "\n";


    startTime = std::chrono::high_resolution_clock::now();
    Malla::acctransf();
    endTime = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "TIEMPO acctransf: " << duration.count() << " microsegundos" << "\n";

    Malla::triplete();

    auto endTimeTotal = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(endTimeTotal - startTimeTotal);
    std::cout << "TIEMPO ITERACION: " << duration.count() << " microsegundos" << "\n";
    cout << "***************************************" << "\n";
}