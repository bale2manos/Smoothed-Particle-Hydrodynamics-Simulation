#ifndef FLUID_FLUID_TEST_HPP
#define FLUID_FLUID_TEST_HPP
#include "sim/block.hpp"
#include <vector>
#include "sim/progargs.hpp"
#include <iostream>


struct TraceBlock {
    long numParticles;
    std::vector<Particle> particles;
};

int compare_iteration(Malla&malla, const char* inputFileName);
int comparevectorstrz(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2);
int comparevectorsout(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2);
std::vector<Particle> traceread(const char* traceFileName);
int compare(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2);
int comparetrzvec(std::vector<Particle>particles_out1, std::vector<Particle>particles_out2);

#endif  // FLUID_FLUID_TEST_HPP