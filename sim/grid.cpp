//
// Created by bale2 on 26/09/2023.
//
#include "grid.hpp"
#include "progargs.hpp"
#include <numbers>
#include <cmath>
#include <unordered_map>
#include <iostream>

/**
 * Constructor for Malla class.
 * @param np Number of particles.
 * @param ppm Particles per meter.
 */
Malla::Malla(int np, double ppm) :
    np(np)
    , ppm(ppm), nBlocks({0,0,0}), sizeBlocks({0,0,0}), h(smooth_length()), m(particle_mass()), accConst({0,0})

  {
  nBlocks = {nx_calc(xmax, xmin, h), ny_calc(ymax, ymin, h), nz_calc(zmax, zmin, h)};
  sizeBlocks = {sx_calc(xmax, xmin, nBlocks[0]), sy_calc(ymax, ymin, nBlocks[1]), sz_calc(zmax, zmin, nBlocks[2])};
  // TODO hacer esto en una funcion o mas bonito
  accConst = {15/(std::numbers::pi*pow(h,6)) * (3*m*p_s)*0.5, 15/(std::numbers::pi*pow(h,6))*3*viscosity*m};
  blocks = createFillGrid();
  particles = vector<Particle>(np);
}

/**
 * Returns a vector containing all the particles in the grid.
 *
 * @return A vector containing all the particles in the grid.
 */
std::vector<Particle> Malla::getParticles() const {
    return particles;
}

/**
 * Returns the value of the private member variable h.
 *
 * @return The value of h.
 */
double Malla::getH() const {
    return h;
}

/**
 * Returns the number of points in the grid.
 *
 * @return The number of points in the grid.
 */
int Malla::getNp() const {
    return np;
}

/**
 * Returns the value of ppm.
 *
 * @return The value of ppm.
 */
double Malla::getPpm() const {
    return ppm;
}

/**
 * Returns the number of blocks in the grid.
 * @return An std::array<int, 3> containing the number of blocks in each dimension.
 */
std::array<int, 3> Malla::getNBlocks() const {
    return nBlocks;
}

/**
 * Returns the size of the blocks in the grid.
 *
 * @return An array of doubles representing the size of the blocks in the grid.
 */
std::array<double, 3> Malla::getSizeBlocks() const {
    return sizeBlocks;
}


/**
 * Calculates the block indexes for a given position in the grid.
 * @param positions An array with the x, y, and z positions.
 * @return An array with the i, j, and k indexes of the block.
 */
array<int, 3> Malla::calculate_block_indexes(array <double,3> positions){
  int i_index = initial_block_index(positions[0], xmin,  sizeBlocks[0]);
  int j_index = initial_block_index(positions[1], ymin,  sizeBlocks[1]);
  int k_index = initial_block_index(positions[2], zmin,  sizeBlocks[2]);
  /* TODO problema coma flotante floor error */
  if (i_index<0){i_index = 0;}
  if (j_index<0){j_index = 0;}
  if (k_index<0){k_index = 0;}
  if (i_index >= nBlocks[0]) {i_index = nBlocks[0] - 1;}
  if (j_index >= nBlocks[1]){j_index = nBlocks[1] - 1;}
  if (k_index >= nBlocks[2]){k_index = nBlocks[2] - 1;}
  return array<int, 3>{i_index,j_index,k_index};
}


/**
 * Inserts particles from a binary file into the grid.
 * 
 * @param in_file The path to the binary file containing the particle information.
 */
void Malla::insert_particles (const char * in_file) {
  ifstream input_file(in_file, ios::binary);
  double trash = 0.0;
  input_file.read(reinterpret_cast<char *>(&trash), sizeof(double));
  array<array<float, 3>, 3> info_particle = {};
  array<array<double, 3>, 3> info_particle_double = {};
  int counter = 0;
  // TODO tratar de reducir reinterpet_cast
  bool finished = false;
  while (!finished){
    for (int i=0;i<3;i++){
      for (int j=0; j<3;j++){
        read_
      }
    }
  }


  while (input_file.read(reinterpret_cast<char *>(info_particle[0].data()), sizeof(info_particle[0][0]))) {
    // if i < np then read the next 8 floats, else continue
    //NOLINTNEXTLINE
    input_file.read(reinterpret_cast<char *>(&info_particle[0][1]), sizeof(info_particle[0][1]));
    input_file.read(reinterpret_cast<char *>(&info_particle[0][2]), sizeof(info_particle[0][2]));
    input_file.read(reinterpret_cast<char *>(info_particle[1].data()), sizeof(info_particle[1][0]));
    input_file.read(reinterpret_cast<char *>(&info_particle[1][1]), sizeof(info_particle[1][1]));
    input_file.read(reinterpret_cast<char *>(&info_particle[1][2]), sizeof(info_particle[1][2]));
    input_file.read(reinterpret_cast<char *>(info_particle[2].data()), sizeof(info_particle[2][0]));
    input_file.read(reinterpret_cast<char *>(&info_particle[2][1]), sizeof(info_particle[2][1]));
    input_file.read(reinterpret_cast<char *>(&info_particle[2][2]), sizeof(info_particle[2][2]));
    for (size_t i = 0; i < info_particle.size(); ++i) {
      for (size_t j = 0; j < info_particle[i].size(); ++j) {
        info_particle_double[i][j] = static_cast<double>(info_particle[i][j]);}
    }

    std::array<int, 3> index_array = calculate_block_indexes(info_particle_double[0]);
    int const index = index_array[0] + index_array[1] * nBlocks[0] + index_array[2] * nBlocks[0] * nBlocks[1];
    insert_particle_info(info_particle_double,counter, index);
    counter++;
  }
  check_missmatch_particles(counter, np);
}


/**
 * Inserts particle information into the grid.
 * @param info An array of arrays containing the position, velocity, and half-velocity of the particle.
 * @param id The ID of the particle.
 * @param block_index The index of the block where the particle is located.
 */
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

/**
 * Calculates the number of cells in the x direction based on the maximum and minimum x values and the cell size.
 * @param x_max The maximum x value.
 * @param x_min The minimum x value.
 * @param h_param The cell size.
 * @return The number of cells in the x direction.
 */
int Malla::nx_calc (double x_max, double x_min, double h_param){
    return floor( (x_max - x_min) / h_param);
}

/**
 * Calculates the number of cells in the y direction based on the maximum and minimum y values and the cell size.
 * @param y_max The maximum y value.
 * @param y_min The minimum y value.
 * @param h_param The cell size.
 * @return The number of cells in the y direction.
 */
int Malla::ny_calc (double y_max, double y_min, double h_param){
    return floor( (y_max - y_min) / h_param);
}

/**
 * Calculates the number of cells in the z direction based on the maximum and minimum z values and the cell size.
 * @param z_max The maximum z value.
 * @param z_min The minimum z value.
 * @param h_param The cell size.
 * @return The number of cells in the z direction.
 */
int Malla::nz_calc (double z_max, double z_min, double h_param){
    return floor( (z_max - z_min) / h_param);
}

/**
 * Calculates the smoothing length of the mesh.
 * @return The smoothing length of the mesh.
 */
double Malla::smooth_length () const{
    return static_cast<double>(radio) / ppm;
}

/**
 * Calculates the x-axis spacing of the mesh.
 * @param x_max The maximum x-coordinate of the mesh.
 * @param x_min The minimum x-coordinate of the mesh.
 * @param n_x The number of cells in the x-axis of the mesh.
 * @return The x-axis spacing of the mesh.
 */
double Malla::sx_calc (double x_max, double x_min, int n_x){
    return (x_max-x_min)/n_x;
}

/**
 * Calculates the y-axis spacing of the mesh.
 * @param y_max The maximum y-coordinate of the mesh.
 * @param y_min The minimum y-coordinate of the mesh.
 * @param n_y The number of cells in the y-axis of the mesh.
 * @return The y-axis spacing of the mesh.
 */
double Malla::sy_calc (double y_max, double y_min, int n_y){
    return (y_max-y_min)/n_y;
}

/**
 * Calculates the z-axis spacing of the mesh.
 * @param z_max The maximum z-coordinate of the mesh.
 * @param z_min The minimum z-coordinate of the mesh.
 * @param n_z The number of cells in the z-axis of the mesh.
 * @return The z-axis spacing of the mesh.
 */
double Malla::sz_calc (double z_max, double z_min, int n_z){
    return (z_max-z_min)/n_z;
}

/**
 * Calculates the mass of a particle in the mesh.
 * @return The mass of a particle in the mesh.
 */
double Malla::particle_mass () const{
    return static_cast<double>(density_factor)/(pow (ppm,3));
}


/**
 * Creates a grid of blocks and appends them to the Malla object.
 * The blocks are created in vectors nx, ny, nz.
 * @return A vector of Block objects representing the created grid.
 */
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


/**
 * Increase the density of two particles if they are close enough.
 *
 * @param pivot_coords The coordinates of the first particle.
 * @param particle2_coords The coordinates of the second particle.
 * @param pivot_rho The density of the first particle.
 * @param particle2_rho The density of the second particle.
 */
void Malla::increase_density(array<double, 3>& pivot_coords, array<double, 3>& particle2_coords, double& pivot_rho, double& particle2_rho) const {
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


/**
 * Calculates the density transformation of a given density value.
 * @param rho The density value to transform.
 * @return The transformed density value.
 */
double Malla::density_transformation(double rho) const{
    const static auto h_sixth = pow(h,6);
    const static auto h_ninth = pow(h,9);
    double const first_term = (rho + h_sixth);
    const static auto second_term = (315/(64*M_PI*h_ninth));
    return first_term*second_term*m;
}


/**
 * Transfers acceleration between two particles if they are close enough.
 * @param pivot The first particle.
 * @param particle2 The second particle.
 */
void Malla::acceleration_transfer(Particle & pivot, Particle & particle2) {
    array<double,3>& position_pivot = pivot.p;
    array<double,3>& position_2 = particle2.p;
    double const differ_x = position_pivot[0] - position_2[0];
    double const differ_y = position_pivot[1] - position_2[1];
    double const differ_z = position_pivot[2] - position_2[2];
    double const norm = sqrt(differ_x * differ_x + differ_y * differ_y + differ_z * differ_z);
    if (norm <= h) {
        const static auto factor    = 1e-6;
        const static auto double_density_f = 2 * density_factor;
        double const distij              = max(norm, factor);
        double const difference_h_distij = h - distij;
        double const term2               = difference_h_distij*difference_h_distij / distij;
        double const density_pivot       = pivot.rho;
        double const density_2           = particle2.rho;
        double const term3               = (density_pivot + density_2 - (double_density_f));
        double const denominator         = 1 / (density_2 * density_pivot);
        double const numerator1          = accConst[0] * term2 * term3;
        for (int i = 0; i < 3; i++) {
            double const term1  = (pivot.p[i] - particle2.p[i]);
            double const term4  = (particle2.v[i] - pivot.v[i]);
            pivot.a[i]         += ((term1 * numerator1) + (term4 * accConst[1])) * denominator;
            particle2.a[i]     -= ((term1 * numerator1) + (term4 * accConst[1])) * denominator;
        }
    }
}


/**
 * Detects and handles collisions between a particle and a block's walls.
 * @param particle The particle to check for collisions.
 * @param block The block to check for collisions against.
 */
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


/**
 * Calculates the collisions of a particle with an edge of the grid in a specific axis.
 * @param particula The particle to check for collisions.
 * @param extremo The edge of the grid to check for collisions (0 for minimum, 1 for maximum).
 * @param eje The axis to check for collisions (0 for x, 1 for y, 2 for z).
 */
void Malla::edge_collisions(Particle& particula, int extremo, int eje) {
  double min_limit = 0.0;
  double max_limit = 0.0;
  const static auto limit_colission = pow(10, -10);
  if (eje == 0){ min_limit= xmin; max_limit = xmax;}
  if (eje == 1){ min_limit= ymin; max_limit = ymax;}
  if (eje == 2){ min_limit= zmin; max_limit = zmax;}

  double const coord = particula.p[eje] + particula.hv[eje] * delta_t;
  if (extremo == 0) {
        double const deltax = d_p - (coord - min_limit);
        if (deltax > limit_colission) {
            particula.a[eje] = particula.a[eje] + (s_c * deltax - d_v * particula.v[eje]);
        }
  } else {
        double const deltax = d_p - (max_limit - coord);
        if (deltax > limit_colission) {
            particula.a[eje] = particula.a[eje] - (s_c * deltax + d_v * particula.v[eje]);
        }
  }
}


/**
 * Moves a particle in the grid by updating its position and velocity based on its acceleration.
 * @param particle The particle to be moved.
 */
void Malla::particle_movement(Particle& particle){

    const static auto delta_t_squared = delta_t * delta_t;
    const static auto delta_t_half = delta_t*0.5;

    for(int i =0;i<3;i++){
        double const hv_i = particle.hv[i];
        double const acc_i = particle.a[i];

        particle.p[i] += (hv_i*delta_t) + (acc_i*delta_t_squared);
        particle.v[i] = hv_i + (acc_i*delta_t_half);
        particle.hv[i] += (acc_i*delta_t);
    }
}


/**
 * Calculates the interaction of a particle with the edges of a given block.
 * 
 * @param particle The particle to interact with the edges.
 * @param block The block to calculate the interaction with.
 */
void Malla::limits_interaction(Particle& particle, Block& block) {
    if (block.coords[0] == 0 || block.coords[0] == nBlocks[0] - 1) { edge_interaction(particle, block.coords[0], 0); }
    if (block.coords[1] == 0 || block.coords[1] == nBlocks[1] - 1) { edge_interaction(particle, block.coords[1], 1); }
    if (block.coords[2] == 0 || block.coords[2] == nBlocks[2] - 1) { edge_interaction(particle, block.coords[2], 2); }
}


/**
 * This function calculates the interaction of a particle with an edge of the grid.
 * @param particle The particle to interact with the edge.
 * @param extremo The index of the edge to interact with (0 for the minimum limit, 1 for the maximum limit).
 * @param eje The index of the axis where the edge is located (0 for x, 1 for y, 2 for z).
 */
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


/**
 * Calculates the density of each particle in the grid by iterating over all particles and their neighbors.
 * For each particle, it increases the density by calling increase_density() function for each of its neighbors.
 * Finally, it applies a density transformation to each particle's density.
 */
void Malla::densinc(){

    for (Particle & particle_pivot: particles){
        int const particle_pivot_id = particle_pivot.id;
        Block  const& current_block =  blocks[particle_pivot.current_block];
        vector<int> const& neighbours_particles = current_block.neighbours_particles;
        for (auto particle2_id: neighbours_particles){
            if(particle_pivot_id < particle2_id){
                Particle & particle2 = particles[particle2_id];
                increase_density(particle_pivot.p, particle2.p, particle_pivot.rho, particle2.rho);
            }
        }
        particle_pivot.rho = density_transformation(particle_pivot.rho);
    }

}


/**
 * Transfer acceleration between particles in the grid.
 * For each particle in the grid, it checks its neighbours and transfers acceleration if necessary.
 * @return void
 */
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


/**
 * This function iterates through all the particles in the grid and checks if they are at the edge of the grid.
 * If a particle is at the edge of the grid, it calls the wall_colissions function to handle the collision with the wall.
 * Then, it calls the particle_movement function to update the position of the particle.
 * If the particle is at the edge of the grid, it calls the limits_interaction function to handle the interaction with the limits of the grid.
 */
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



/**
 * This function updates the position of the particles in the grid and calculates the new block indexes.
 * It also initializes the acceleration and density of each particle.
 * Finally, it updates the list of particles in each block and the list of particles in the neighboring blocks.
 */
void Malla::repos(){
    for (Block & block : blocks) {
      block.particles.clear();
      block.neighbours_particles.clear();
    }

    for (Particle & particle : particles) {
      array<int,3> const new_indexes = calculate_block_indexes(particle.p);
      int const block_index = calculate_block_index(new_indexes, nBlocks[0], nBlocks[1]);
      particle.current_block = block_index;
      particle.rho = 0;
      particle.a = {0, gravity, 0};

      auto new_block = static_cast<size_t>(block_index);
      blocks[new_block].particles.emplace_back(particle.id);
    }

    add_particles_neighbours();
}


/**
 * This function adds the particles in the neighboring blocks to the list of particles in each block.
 */
void Malla::add_particles_neighbours() {
    int currentBlock = 0;
    for (Block  const& block: blocks){
        for (auto index: block.neighbours){
            Block const& neighbour_block = blocks[index];
            for (auto particle_id: neighbour_block.particles){
               blocks[currentBlock].neighbours_particles.emplace_back(particle_id);
            }
        }
        currentBlock++;
    }
}

/**
 * This method performs the interaction between the different particles in the grid.
 * It calls the following methods in order: repos(), densinc(), acctransf(), and triplete().
 */
void Malla::mallaInteraction(){
    Malla::repos();
    Malla::densinc();
    Malla::acctransf();
    Malla::triplete();
}