//
// Created by bale2 on 26/09/2023.
//

#ifndef ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP
#define ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP

#include <fstream>
using namespace std;

int validate_parameters (int argc, const char * argv[]);
int validate_time_steps (int nts);
int validate_input_file(const char* inputFileName);
int validate_output_file(const char* outputFileName);
int read_input_file(const char * in_file);
double smooth_length (double ppm);
double particle_mass (double ppm);
double nx_calc (double xmax, double xmin, double h);
double ny_calc (double ymax, double ymin, double h);
double nz_calc (double zmax, double zmin, double h);
double sx_calc (double xmax, double xmin, double nx);
double sy_calc (double ymax, double ymin, double ny);
double sz_calc (double zmax, double zmin, double nz);

#endif //ARQUITECTURA_DE_COMPUTADORES_PROGARGS_HPP