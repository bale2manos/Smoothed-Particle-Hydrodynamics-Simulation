//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP
#define ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP

#include "grid.hpp"

#include <array>
#include <fstream>
#include <vector>

std::array<int, 2> validate_parameters(int argc, std::vector<std::string> argv);
int validate_time_steps(int nts);
int validate_input_file(char const * inputFileName);
int validate_output_file(char const * outputFileName);
Malla read_input_file(char const * in_file);
void check_number_particles(int number_particles);
int write_output_file(Malla & malla, char const * out_file);
void check_missmatch_particles(int counter, int malla_np);
void read_particle_field(bool & finished, std::array<int, 2> indexes, std::ifstream & input_file,
                         std::array<std::array<float, 3>, 3> & info_particle);
void write_output_field(std::ofstream & output_file, auto & field_to_write);
void read_field_from_file(std::ifstream & input_file, auto & field_to_read);

#endif  // ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP
