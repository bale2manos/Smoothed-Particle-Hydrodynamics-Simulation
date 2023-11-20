#include "progargs.hpp"

#include "block.hpp"
#include "grid.hpp"

#include <fstream>
#include <iostream>
#include <string>

/**
 * Validates the number of time steps.
 * @param nts The number of time steps to validate.
 * @return 0 if the number of time steps is valid, -1 if it's not numeric, -2 if it's negative.
 */
int validate_time_steps(int nts) {
  if (nts <= 0) {
    std::cerr << "Error: Invalid number of time steps."
              << "\n";
    return -2;
  }
  return 0;
}

/**
 * Validates if the input file can be opened for reading.
 *
 * @param inputFileName The name of the input file to be validated.
 * @return Returns 0 if the input file can be opened for reading, -1 otherwise.
 */
int validate_input_file(char const * inputFileName) {
  std::cout << "Input file: " << inputFileName << "\n";
  std::ifstream input_file(inputFileName);
  if (!input_file.is_open()) {
    std::cerr << "Error: Cannot open " << inputFileName << " for reading."
              << "\n";
    return -1;
  }

  return 0;
}

/**
 * Validates if the output file can be opened for writing.
 * @param outputFileName The name of the output file.
 * @return 0 if the file can be opened for writing, -1 otherwise.
 */
int validate_output_file(char const * outputFileName) {
  std::ofstream output_file(outputFileName);
  if (!output_file.is_open()) {
    std::cerr << "Error: Cannot open " << outputFileName << " for writing."
              << "\n";
    return -1;
  }
  return 0;
}

/**
 * Validates the parameters passed to the program.
 * @param argc The number of arguments passed to the program.
 * @param argv An array of C-style strings containing the arguments passed to the program.
 * @return An array of integers representing the error type. If the first element is 0, there is no
 * error. If it is negative, there is an error. The second element contains the number of time
 * steps.
 */
std::array<int, 2> validate_parameters(int argc, std::vector<std::string> argv) {
  std::array<int, 2> error_type = {0, 0};
  if (argc != 4) {
    std::cerr << "Error: Invalid number of arguments: " << argc << "\n";
    error_type[0] = -1;
    return error_type;
  }
  try {
    error_type[1] = stoi(argv[0]);
  } catch (std::invalid_argument const & e) {
    std::cerr << "Error time steps must be numeric.\n";
    error_type[0] = -1;
    return error_type;
  }
  error_type[0] = validate_time_steps(error_type[1]);
  if (error_type[0] != 0) { return error_type; }
  if (validate_input_file(argv[1].c_str()) != 0) {
    error_type[0] = -3;
    return error_type;
  }
  if (validate_output_file(argv[2].c_str()) != 0) {
    error_type[0] = -4;
    return error_type;
  }
  return error_type;
}

/**
 * Reads an input file and creates a Malla object with the particle data.
 *
 * @param in_file The path to the input file.
 * @return A Malla object with the particle data.
 */
Malla read_input_file(char const * in_file) {
  std::ifstream input_file(in_file, std::ios::binary);

  float particles_per_meter = 0;
  read_field_from_file(input_file, particles_per_meter);
  int number_particles = 0;
  read_field_from_file(input_file, number_particles);
  check_number_particles(number_particles);
  auto particles_per_meter_double = static_cast<double>(particles_per_meter);

  Malla grid(number_particles, particles_per_meter_double);
  grid.insert_particles(in_file);

  std::cout << "Number of particles: " << number_particles << "\n";
  std::cout << "Particles per meter: " << particles_per_meter_double << "\n";
  std::cout << "Smoothing length: " << grid.getSmoothingLength() << "\n";
  std::cout << "Particle mass: " << grid.particle_mass_calc() << "\n";
  std::cout << "Grid size: " << grid.getNumberBlocks()[0] << " x " << grid.getNumberBlocks()[1]
            << " x " << grid.getNumberBlocks()[2] << "\n";
  std::cout << "Number of blocks: "
            << grid.getNumberBlocks()[0] * grid.getNumberBlocks()[1] * grid.getNumberBlocks()[2]
            << "\n";
  std::cout << "Block size: " << grid.getSizeBlocks()[0] << " x " << grid.getSizeBlocks()[1]
            << " x " << grid.getSizeBlocks()[2] << "\n";
  return grid;
}

void read_field_from_file(std::ifstream & input_file, auto & field_to_read) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input_file.read(reinterpret_cast<char *>(&field_to_read), sizeof(field_to_read));
}

/**
 * Writes the particle data to a binary file.
 *
 * @param malla The mesh containing the particles.
 * @param out_file The output file path.
 * @return 0 if the file was successfully written, -1 otherwise.
 */
int write_output_file(Malla & malla, char const * out_file) {
  int number_particles       = malla.getNumberParticles();
  double particles_per_meter = malla.getParticlesPerMeter();
  std::ofstream output_file(out_file, std::ios::binary);
  write_output_field(output_file, particles_per_meter);
  write_output_field(output_file, number_particles);
  std::array<float, nine> particle_info = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  for (Particle & particle : malla.getParticles()) {
    for (std::array<float, 3>::size_type i = 0; i < 3; i++) {
      particle_info.at(i) = static_cast<float>(particle.pos.at(i));
    }

    for (std::array<float, 3>::size_type i = 0; i < 3; i++) {
      particle_info.at(i + 3) = static_cast<float>(particle.h_vel.at(i));
    }

    for (std::array<float, 3>::size_type i = 0; i < 3; i++) {
      particle_info.at(i + 3 + 3) = static_cast<float>(particle.vel.at(i));
    }
    for (float & field : particle_info) { write_output_field(output_file, field); }
  }
  return 0;
}

/**
 * Writes a field from the particle to the output file.
 * @param output_file The output file stream.
 * @param field_to_write The field from the particle to write.
 */
void write_output_field(std::ofstream & output_file, auto & field_to_write) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output_file.write(reinterpret_cast<char *>(&field_to_write), sizeof(field_to_write));
}

/**
 * Checks if the number of particles is valid.
 * @param np The number of particles to check.
 * @throws runtime_error if the number of particles is less than or equal to zero.
 */
void check_number_particles(int number_particles) {
  if (number_particles <= 0) {
    std::string const errorMsg =
        "Error: Invalid number of particles: " + std::to_string(number_particles) + ".\n";
    throw std::runtime_error(errorMsg);
  }
}

/**
 * Checks if the number of particles in the header matches the number of particles found.
 * @param counter The number of particles found.
 * @param malla_np The number of particles in the header.
 * @throws runtime_error if the number of particles does not match.
 */
void check_missmatch_particles(int counter, int malla_np) {
  if (counter != malla_np) {
    std::string const errorMsg =
        "Error: Number of particles mismatch. Header: " + std::to_string(malla_np) +
        ", Found: " + std::to_string(counter) + ".\n";
    throw std::runtime_error(errorMsg);
  }
}

/**
 * Reads the particle data from the input file and inserts it into the grid.
 * @param in_file The path to the input file.
 */
void read_particle_field(bool & finished, std::array<int, 2> indexes, std::ifstream & input_file,
                         std::array<std::array<float, 3>, 3> & info_particle) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (!input_file.read(reinterpret_cast<char *>(&info_particle.at(indexes[0])[indexes[1]]),
                       sizeof(info_particle.at(indexes[0])[indexes[1]]))) {
    finished = true;
  }
}
