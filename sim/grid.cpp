//
// Created by bale2 on 26/09/2023.
//
#include "grid.hpp"

#include "progargs.hpp"

#include <cmath>
#include <iostream>
#include <numbers>
#include <unordered_map>

/**
 * Constructor for Malla class.
 * @param np Number of particles.
 * @param ppm Particles per meter.
 */
Malla::Malla(int np, double ppm)
  : number_particles(np), particles_per_meter(ppm), number_blocks({0, 0, 0}),
    size_blocks({0, 0, 0}), smoothing_length(smooth_length_calc()),
    particle_mass(particle_mass_calc()), accel_constants({0, 0})

{
  number_blocks = {nx_calc(xmax, xmin, smoothing_length), ny_calc(ymax, ymin, smoothing_length),
                   nz_calc(zmax, zmin, smoothing_length)};
  size_blocks   = {sx_calc(xmax, xmin, number_blocks[0]), sy_calc(ymax, ymin, number_blocks[1]),
                   sz_calc(zmax, zmin, number_blocks[2])};
  // TODO hacer esto en una funcion o mas bonito
  accel_constants = {fifteen / (std::numbers::pi * pow(smoothing_length, six)) *
                         (3 * particle_mass * stiffness_presure) * half,
                     fifteen / (std::numbers::pi * pow(smoothing_length, six)) * three * viscosity *
                         particle_mass};
  blocks          = createFillGrid();
  particles       = std::vector<Particle>(np);
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
double Malla::getSmoothingLength() const {
  return smoothing_length;
}

/**
 * Returns the number of points in the grid.
 *
 * @return The number of points in the grid.
 */
int Malla::getNumberParticles() const {
  return number_particles;
}

/**
 * Returns the value of particles_per_meter.
 *
 * @return The value of particles_per_meter.
 */
double Malla::getParticlesPerMeter() const {
  return particles_per_meter;
}

/**
 * Returns the number of blocks in the grid.
 * @return An std::array<int, 3> containing the number of blocks in each dimension.
 */
std::array<int, 3> Malla::getNumberBlocks() const {
  return number_blocks;
}

/**
 * Returns the size of the blocks in the grid.
 *
 * @return An array of doubles representing the size of the blocks in the grid.
 */
std::array<double, 3> Malla::getSizeBlocks() const {
  return size_blocks;
}

/**
 * Calculates the block indexes for a given position in the grid.
 * @param positions An array with the x, y, and z positions.
 * @return An array with the i, j, and k indexes of the block.
 */
std::array<int, 3> Malla::calculate_block_indexes(std::array<double, 3> positions) {
  int i_index = initial_block_index(positions[0], xmin, size_blocks[0]);
  int j_index = initial_block_index(positions[1], ymin, size_blocks[1]);
  int k_index = initial_block_index(positions[2], zmin, size_blocks[2]);
  if (i_index < 0) { i_index = 0; }
  if (j_index < 0) { j_index = 0; }
  if (k_index < 0) { k_index = 0; }
  if (i_index >= number_blocks[0]) { i_index = number_blocks[0] - 1; }
  if (j_index >= number_blocks[1]) { j_index = number_blocks[1] - 1; }
  if (k_index >= number_blocks[2]) { k_index = number_blocks[2] - 1; }
  return std::array<int, 3>{i_index, j_index, k_index};
}

/**
 * Inserts particles from a binary file into the grid.
 *
 * @param in_file The path to the binary file containing the particle information.
 */
void Malla::insert_particles(char const * in_file) {
  std::ifstream input_file(in_file, std::ios::binary);
  double header_already_read = 0.0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input_file.read(reinterpret_cast<char *>(&header_already_read), sizeof(double));
  std::array<std::array<float, 3>, 3> info_particle         = {};
  std::array<std::array<double, 3>, 3> info_particle_double = {};
  int particle_counter                                      = 0;
  bool whole_file_read                                      = false;
  while (!whole_file_read) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        std::array<int, 2> const indexes = {i, j};
        read_particle_field(whole_file_read, indexes, input_file, info_particle);
        info_particle_double.at(i).at(j) = static_cast<double>(info_particle.at(i).at(j));
        if (whole_file_read) { break; }
      }
      if (whole_file_read) { break; }
    }
    if (whole_file_read) { break; }
    int const index = get_block_index_from_position(info_particle_double[0]);
    insert_particle_info(info_particle_double, particle_counter, index);
    particle_counter++;
  }
  check_missmatch_particles(particle_counter, number_particles);
}

int Malla::get_block_index_from_position(std::array<double, 3> position) {
  std::array<int, 3> index_array = calculate_block_indexes(position);
  return index_array[0] + index_array[1] * number_blocks[0] +
         index_array[2] * number_blocks[0] * number_blocks[1];
}

/**
 * Inserts particle information into the grid.
 * @param info An array of arrays containing the position, velocity, and half-velocity of the
 * particle.
 * @param id The ID of the particle.
 * @param block_index The index of the block where the particle is located.
 */
void Malla::insert_particle_info(std::array<std::array<double, 3>, 3> info, int id,
                                 int block_index) {
  Particle particle{};
  particle.pos           = info[0];
  particle.h_vel         = info[1];
  particle.vel           = info[2];
  particle.acc           = {0, 0, 0};
  particle.density       = 0;
  particle.id            = id;
  particle.current_block = block_index;
  blocks[block_index].particles.push_back(id);
  particles[id] = particle;
}

/**
 * Calculates the number of cells in the x direction based on the maximum and minimum x values and
 * the cell size.
 * @param x_max The maximum x value.
 * @param x_min The minimum x value.
 * @param h_param The cell size.
 * @return The number of cells in the x direction.
 */
int Malla::nx_calc(double x_max, double x_min, double h_param) {
  return floor((x_max - x_min) / h_param);
}

/**
 * Calculates the number of cells in the y direction based on the maximum and minimum y values and
 * the cell size.
 * @param y_max The maximum y value.
 * @param y_min The minimum y value.
 * @param h_param The cell size.
 * @return The number of cells in the y direction.
 */
int Malla::ny_calc(double y_max, double y_min, double h_param) {
  return floor((y_max - y_min) / h_param);
}

/**
 * Calculates the number of cells in the z direction based on the maximum and minimum z values and
 * the cell size.
 * @param z_max The maximum z value.
 * @param z_min The minimum z value.
 * @param h_param The cell size.
 * @return The number of cells in the z direction.
 */
int Malla::nz_calc(double z_max, double z_min, double h_param) {
  return floor((z_max - z_min) / h_param);
}

/**
 * Calculates the smoothing length of the mesh.
 * @return The smoothing length of the mesh.
 */
double Malla::smooth_length_calc() const {
  return static_cast<double>(radio_multiplier) / particles_per_meter;
}

/**
 * Calculates the x-axis spacing of the mesh.
 * @param x_max The maximum x-coordinate of the mesh.
 * @param x_min The minimum x-coordinate of the mesh.
 * @param n_x The number of cells in the x-axis of the mesh.
 * @return The x-axis spacing of the mesh.
 */
double Malla::sx_calc(double x_max, double x_min, int n_x) {
  return (x_max - x_min) / n_x;
}

/**
 * Calculates the y-axis spacing of the mesh.
 * @param y_max The maximum y-coordinate of the mesh.
 * @param y_min The minimum y-coordinate of the mesh.
 * @param n_y The number of cells in the y-axis of the mesh.
 * @return The y-axis spacing of the mesh.
 */
double Malla::sy_calc(double y_max, double y_min, int n_y) {
  return (y_max - y_min) / n_y;
}

/**
 * Calculates the z-axis spacing of the mesh.
 * @param z_max The maximum z-coordinate of the mesh.
 * @param z_min The minimum z-coordinate of the mesh.
 * @param n_z The number of cells in the z-axis of the mesh.
 * @return The z-axis spacing of the mesh.
 */
double Malla::sz_calc(double z_max, double z_min, int n_z) {
  return (z_max - z_min) / n_z;
}

/**
 * Calculates the mass of a particle in the mesh.
 * @return The mass of a particle in the mesh.
 */
double Malla::particle_mass_calc() const {
  return static_cast<double>(fluid_density) / (pow(particles_per_meter, 3));
}

/**
 * Creates a grid of blocks and appends them to the Malla object.
 * The blocks are created in vectors nx, ny, nz.
 * @return A vector of Block objects representing the created grid.
 */
std::vector<Block> Malla::createFillGrid() {
  // Create the blocks and append them to Malla. Create all vectors in nx, ny, nz
  for (int k = 0; k < number_blocks[2]; ++k) {
    for (int j = 0; j < number_blocks[1]; ++j) {
      for (int i = 0; i < number_blocks[0]; ++i) {
        std::array<int, 3> const indexes = {i, j, k};
        blocks.emplace_back(indexes, number_blocks);
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
void Malla::increase_density(std::array<double, 3> & pivot_coords,
                             std::array<double, 3> & particle2_coords, double & pivot_rho,
                             double & particle2_rho) const {
  static auto const h_squared = smoothing_length * smoothing_length;
  double const dx_differ      = pivot_coords[0] - particle2_coords[0];
  double const dy_differ      = pivot_coords[1] - particle2_coords[1];
  double const dz_differ      = pivot_coords[2] - particle2_coords[2];
  double const norm_squared = dx_differ * dx_differ + dy_differ * dy_differ + dz_differ * dz_differ;
  if (norm_squared < h_squared) {
    double const diff      = (h_squared - norm_squared);
    double const increase  = diff * diff * diff;
    pivot_rho             += increase;
    particle2_rho         += increase;
  }
}

/**
 * Calculates the density transformation of a given density value.
 * @param rho The density value to transform.
 * @return The transformed density value.
 */
double Malla::density_transformation(double rho) const {
  static auto const h_sixth     = pow(smoothing_length, 6);
  static auto const h_ninth     = pow(smoothing_length, 9);
  double const first_term       = (rho + h_sixth);
  static auto const second_term = (315 / (64 * M_PI * h_ninth));
  return first_term * second_term * particle_mass;
}

/**
 * Transfers acceleration between two particles if they are close enough.
 * @param pivot The first particle.
 * @param particle2 The second particle.
 */
void Malla::acceleration_transfer_calculations(Particle & pivot, Particle & particle2) {
  std::array<double, 3> & position_pivot = pivot.pos;
  std::array<double, 3> & position_2     = particle2.pos;
  double const differ_x                  = position_pivot[0] - position_2[0];
  double const differ_y                  = position_pivot[1] - position_2[1];
  double const differ_z                  = position_pivot[2] - position_2[2];
  double const norm = sqrt(differ_x * differ_x + differ_y * differ_y + differ_z * differ_z);
  if (norm <= smoothing_length) {
    static auto const factor           = 1e-6;
    static auto const double_density_f = 2 * fluid_density;
    double const particles_distance    = std::max(norm, factor);
    double const difference_h_distij   = smoothing_length - particles_distance;
    double const term2         = difference_h_distij * difference_h_distij / particles_distance;
    double const density_pivot = pivot.density;
    double const density_2     = particle2.density;
    double const term3         = (density_pivot + density_2 - (double_density_f));
    double const denominator   = 1 / (density_2 * density_pivot);
    double const numerator1    = accel_constants[0] * term2 * term3;
    for (int i = 0; i < 3; i++) {
      double const term1   = (pivot.pos.at(i) - particle2.pos.at(i));
      double const term4   = (particle2.vel.at(i) - pivot.vel.at(i));
      pivot.acc.at(i)     += ((term1 * numerator1) + (term4 * accel_constants[1])) * denominator;
      particle2.acc.at(i) -= ((term1 * numerator1) + (term4 * accel_constants[1])) * denominator;
    }
  }
}

/**
 * Detects and handles collisions between a particle and a block's walls.
 * @param particle The particle to check for collisions.
 * @param block The block to check for collisions against.
 */
void Malla::wall_colissions(Particle & particle, Block & block) {
  if (block.coords[0] == 0 || block.coords[0] == number_blocks[0] - 1) {
    edge_collisions(particle, block.coords[0], 0);
  }
  if (block.coords[1] == 0 || block.coords[1] == number_blocks[1] - 1) {
    edge_collisions(particle, block.coords[1], 1);
  }
  if (block.coords[2] == 0 || block.coords[2] == number_blocks[2] - 1) {
    edge_collisions(particle, block.coords[2], 2);
  }
}

/**
 * Calculates the collisions of a particle with an edge of the grid in a specific axis.
 * @param particula The particle to check for collisions.
 * @param extremo The edge of the grid to check for collisions (0 for minimum, 1 for maximum).
 * @param eje The axis to check for collisions (0 for x, 1 for y, 2 for z).
 */
void Malla::edge_collisions(Particle & particula, int extremo, int eje) {
  static auto const limit_colission = pow(10, -10);

  double const coord = particula.pos.at(eje) + particula.h_vel.at(eje) * timestep;
  if (extremo == 0) {
    double const min_limit = limit_of_edge(eje, extremo);
    double const deltax    = particle_size - (coord - min_limit);
    if (deltax > limit_colission) {
      particula.acc.at(eje) += (rigidity_collisions * deltax - damping * particula.vel.at(eje));
    }
  } else {
    double const max_limit = limit_of_edge(eje, extremo);
    double const deltax    = particle_size - (max_limit - coord);
    if (deltax > limit_colission) {
      particula.acc.at(eje) += -(rigidity_collisions * deltax + damping * particula.vel.at(eje));
    }
  }
}

/**
 * Moves a particle in the grid by updating its position and velocity based on its acceleration.
 * @param particle The particle to be moved.
 */
void Malla::particle_movement(Particle & particle) {
  static auto const delta_t_squared = timestep * timestep;
  static auto const delta_t_half    = timestep * 0.5;

  for (int i = 0; i < 3; i++) {
    double const hv_i  = particle.h_vel.at(i);
    double const acc_i = particle.acc.at(i);

    particle.pos.at(i)   += (hv_i * timestep) + (acc_i * delta_t_squared);
    particle.vel.at(i)    = hv_i + (acc_i * delta_t_half);
    particle.h_vel.at(i) += (acc_i * timestep);
  }
}

/**
 * Calculates the interaction of a particle with the edges of a given block.
 *
 * @param particle The particle to interact with the edges.
 * @param block The block to calculate the interaction with.
 */
void Malla::limits_interaction(Particle & particle, Block & block) {
  if (block.coords[0] == 0 || block.coords[0] == number_blocks[0] - 1) {
    edge_interaction(particle, block.coords[0], 0);
  }
  if (block.coords[1] == 0 || block.coords[1] == number_blocks[1] - 1) {
    edge_interaction(particle, block.coords[1], 1);
  }
  if (block.coords[2] == 0 || block.coords[2] == number_blocks[2] - 1) {
    edge_interaction(particle, block.coords[2], 2);
  }
}

/**
 * This function calculates the interaction of a particle with an edge of the grid.
 * @param particle The particle to interact with the edge.
 * @param extremo The index of the edge to interact with (0 for the minimum limit, 1 for the maximum
 * limit).
 * @param eje The index of the axis where the edge is located (0 for x, 1 for y, 2 for z).
 */
void Malla::edge_interaction(Particle & particle, int extremo, int eje) {
  double displacement_edge = 0.0;
  if (extremo == 0) {
    double const min_limit = limit_of_edge(eje, extremo);
    displacement_edge      = particle.pos.at(eje) - min_limit;
    if (displacement_edge < 0) { particle.pos.at(eje) = min_limit - displacement_edge; }
  } else {
    double const max_limit = limit_of_edge(eje, extremo);
    displacement_edge      = max_limit - particle.pos.at(eje);
    if (displacement_edge < 0) { particle.pos.at(eje) = max_limit + displacement_edge; }
  }
  if (displacement_edge < 0) {
    particle.vel.at(eje)   *= -1;
    particle.h_vel.at(eje) *= -1;
  }
}

/**
 * Calculates the density of each particle in the grid by iterating over all particles and their
 * neighbors. For each particle, it increases the density by calling increase_density() function for
 * each of its neighbors. Finally, it applies a density transformation to each particle's density.
 */
void Malla::density_increase_and_transformation() {
  for (Particle & particle_pivot : particles) {
    int const particle_pivot_id                   = particle_pivot.id;
    Block const & current_block                   = blocks[particle_pivot.current_block];
    std::vector<int> const & neighbours_particles = current_block.neighbours_particles;
    for (auto particle2_id : neighbours_particles) {
      if (particle_pivot_id < particle2_id) {
        Particle & particle2 = particles[particle2_id];
        increase_density(particle_pivot.pos, particle2.pos, particle_pivot.density,
                         particle2.density);
      }
    }
    particle_pivot.density = density_transformation(particle_pivot.density);
  }
}

/**
 * Transfer acceleration between particles in the grid.
 * For each particle in the grid, it checks its neighbours and transfers acceleration if necessary.
 * @return void
 */
void Malla::acceleration_transfer() {
  for (Particle & particle_pivot : particles) {
    int const particle_pivot_id                   = particle_pivot.id;
    Block const & current_block                   = blocks[particle_pivot.current_block];
    std::vector<int> const & neighbours_particles = current_block.neighbours_particles;
    for (auto particle2_id : neighbours_particles) {
      if (particle_pivot_id < particle2_id) {
        Particle & particle2 = particles[particle2_id];
        acceleration_transfer_calculations(particle_pivot, particle2);
      }
    }
  }
}

/**
 * This function iterates through all the particles in the grid and checks if they are at the edge
 * of the grid. If a particle is at the edge of the grid, it calls the wall_colissions function to
 * handle the collision with the wall. Then, it calls the particle_movement function to update the
 * position of the particle. If the particle is at the edge of the grid, it calls the
 * limits_interaction function to handle the interaction with the limits of the grid.
 */
void Malla::particle_collide_move_and_bound_interact() {
  std::array<int, 3> const n_blocks = number_blocks;
  for (Particle & particle_pivot : particles) {
    Block & block = blocks[particle_pivot.current_block];
    bool extremo  = false;

    if (block.coords[0] == 0 || block.coords[1] == 0 || block.coords[2] == 0 ||
        block.coords[0] == n_blocks[0] - 1 || block.coords[1] == n_blocks[1] - 1 ||
        block.coords[2] == n_blocks[2] - 1) {
      extremo = true;
      wall_colissions(particle_pivot, block);
    }
    particle_movement(particle_pivot);
    if (extremo) { limits_interaction(particle_pivot, block); }
  }
}

/**
 * This function updates the position of the particles in the grid and calculates the new block
 * indexes. It also initializes the acceleration and density of each particle. Finally, it updates
 * the list of particles in each block and the list of particles in the neighboring blocks.
 */
void Malla::particle_repositioning() {
  for (Block & block : blocks) {
    block.particles.clear();
    block.neighbours_particles.clear();
  }

  for (Particle & particle : particles) {
    int const block_index  = get_block_index_from_position(particle.pos);
    particle.current_block = block_index;
    particle.density       = 0;
    particle.acc           = {0, gravity, 0};

    auto new_block = static_cast<size_t>(block_index);
    blocks[new_block].particles.emplace_back(particle.id);
  }

  add_particles_neighbours();
}

/**
 * This function adds the particles in the neighboring blocks to the list of particles in each
 * block.
 */
void Malla::add_particles_neighbours() {
  int currentBlock = 0;
  for (Block const & block : blocks) {
    for (auto index : block.neighbours) {
      Block const & neighbour_block = blocks[index];
      for (auto particle_id : neighbour_block.particles) {
        blocks[currentBlock].neighbours_particles.emplace_back(particle_id);
      }
    }
    currentBlock++;
  }
}

/**
 * This method performs the interaction between the different particles in the grid.
 * It calls the following methods in order: particle_repositioning(),
 * density_increase_and_transformation(), acceleration_transfer(), and
 * particle_collide_move_and_bound_interact().
 */
void Malla::mallaInteraction() {
  Malla::particle_repositioning();
  Malla::density_increase_and_transformation();
  Malla::acceleration_transfer();
  Malla::particle_collide_move_and_bound_interact();
}

double Malla::limit_of_edge(int eje, int extremo) {
  if (eje == 0) {
    if (extremo == 0) { return xmin; }
    return xmax;
  }
  if (eje == 1) {
    if (extremo == 0) { return ymin; }
    return ymax;
  }
  if (eje == 2) {
    if (extremo == 0) { return zmin; }
    return zmax;
  }
  return 0.0;
}