#ifndef FLUID_FLUID_TEST_HPP
#define FLUID_FLUID_TEST_HPP
#include "sim/block.hpp"
#include "sim/progargs.hpp"

#include <iostream>
#include <vector>

struct TraceBlock {
    long numParticles;
    std::vector<Particle> particles;
};

int compare_iteration(Malla & malla, char const * inputFileName);
int comparevectorstrz(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2);
int comparevectorsout(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2);
std::vector<Particle> traceread(char const * traceFileName);
int compare(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2);
int comparetrzvec(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2);
void read_field_from_file_double(std::ifstream & input_file, auto & field_to_read);
void read_field_from_file_long(std::ifstream & input_file, auto & field_to_read);
void read_field_from_file_int(std::ifstream & input_file, auto & field_to_read);
Particle makeparticle(std::ifstream & traceFile, int iter);

void read_field_from_file2(std::ifstream & input_file, auto & field_to_read);
#endif  // FLUID_FLUID_TEST_HPP