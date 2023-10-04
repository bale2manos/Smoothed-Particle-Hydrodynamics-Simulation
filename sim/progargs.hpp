//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP
#define ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP

#include <fstream>
#include <vector>
#include "grid.hpp"
using namespace std;





int validate_parameters (int argc, const char * argv[]);
int validate_time_steps (int nts);
int validate_input_file(const char* inputFileName);
int validate_output_file(const char* outputFileName);
Malla read_input_file(const char * in_file);
void refactor_gordo(ifstream input_file);
void check_np(int np);

int write_output_file (Malla malla, const char * out_file);


struct ParticleFloat{
    float px_float, py_float, pz_float, hvx_float, hvy_float, hvz_float, vx_float, vy_float, vz_float;
    ifstream in_file;
};



#endif //ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP


