//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP
#define ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP

#include <fstream>
#include <vector>
#include <array>
#include "grid.hpp"
using namespace std;





array<int,2> validate_parameters (int argc, const char * argv[]);
int validate_time_steps (int nts);
int validate_input_file(const char* inputFileName);
int validate_output_file(const char* outputFileName);
void read_input_file (Malla& malla, const char * in_file, vector<Particle>& particles);
void refactor_gordo (const char * in_file, Malla& malla, vector<Particle>& particles);
void check_np(int np);
int write_output_file (Malla& malla, const char * out_file);
array<int, 3> calculate_block_indexes(array <double,3> positions, Malla& malla);
void insert_particle_info(array<array<double, 3>, 3> info, Block& bloque, int id, int block_index, vector<Particle>& particles);
void check_missmatch_particles(int counter, int malla_np);



#endif //ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP


