//
// Created by carlos on 6/11/23.
//

#include <iostream>
#include <vector>
#include "sim/block.hpp"
#include "sim/progargs.hpp"
#include "fluid_test.hpp"
#include <gtest/gtest.h>


int compare_iteration(Malla&malla, const char* inputFileName){
  vector<Particle> particles_out1, particles_out2;
  particles_out2 = traceread(inputFileName);

  for (Block & block1 : malla.blocks) {
    // Loop through all the particles in the block
    for (Particle & particle : block1.particles) {
      // Cast from double to float and write to file
      particles_out1.push_back(particle);
    }
  }
  sort(particles_out1.begin(), particles_out1.end(), [](Particle & a, Particle & b) { return a.id < b.id; });
  sort(particles_out2.begin(), particles_out2.end(), [](Particle & a, Particle & b) { return a.id < b.id; });

  if (particles_out1.size() != particles_out2.size()){
    return -1;
  }
  for (int y = 0; y < int (particles_out1.size()); y++ ){
    if (particles_out1[y].id != particles_out2[y].id || particles_out1[y].current_block != particles_out2[y].current_block||
        std::abs(particles_out1[y].rho - particles_out2[y].rho) >  exp(-3)|| std::abs(particles_out1[y].p[0] - particles_out2[y].p[0]) > exp(-3)||
        std::abs(particles_out1[y].p[1] - particles_out2[y].p[1]) > exp(-3)||std::abs(particles_out1[y].p[2] - particles_out2[y].p[2]) > exp(-3)||
        std::abs(particles_out1[y].hv[0] - particles_out2[y].hv[0]) > exp(-3) ||std::abs(particles_out1[y].hv[1] - particles_out2[y].hv[1]) > exp(-3) ||std::abs(particles_out1[y].hv[2] - particles_out2[y].hv[2]) > exp(-3) ||
        std::abs(particles_out1[y].v[0] - particles_out2[y].v[0]) > exp(-3) || std::abs(particles_out1[y].v[1] - particles_out2[y].v[1]) > exp(-3) ||std::abs(particles_out1[y].v[2] - particles_out2[y].v[2]) > exp(-3) ||
        std::abs(particles_out1[y].a[0] - particles_out2[y].a[0]) > exp(-3) || std::abs(particles_out1[y].a[0] - particles_out2[y].a[0]) > exp(-3)|| std::abs(particles_out1[y].a[0] - particles_out2[y].a[0]) > exp(-3)){
      return -1;
    }
  }
  return 0;

}



vector<Particle> traceread(const char* traceFileName) {
  std::cout << "HOLA MUNDO" << std::endl;
  std::ifstream traceFile(traceFileName, std::ios::binary);

  if (!traceFile) {
    string errorMsg = "Error al abrir el archivo de trazas.";
    throw runtime_error(errorMsg);

  }

  int numBlocks;
  traceFile.read(reinterpret_cast<char*>(&numBlocks), sizeof(int));

  std::vector<TraceBlock> blocks;

  for (int i = 0; i < numBlocks; i++) {
    TraceBlock block;
    traceFile.read(reinterpret_cast<char*>(&block.numParticles), sizeof(long long));

    for (long j = 0; j < block.numParticles; j++) {
      Particle particle;
      traceFile.read(reinterpret_cast<char*>(&particle.id), sizeof(long long));
      traceFile.read(reinterpret_cast<char*>(&particle.p[0]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.p[1]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.p[2]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.hv[0]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.hv[1]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.hv[2]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.v[0]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.v[1]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.v[2]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.rho), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.a[0]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.a[1]), sizeof(double));
      traceFile.read(reinterpret_cast<char*>(&particle.a[2]), sizeof(double));
      particle.current_block = i;

      block.particles.push_back(particle);
    }

    blocks.push_back(block);
  }

  // Ahora puedes trabajar con los bloques y las partículas
  std::vector<Particle> particles_out;
  // Loop through all the blocks
  for (TraceBlock & block : blocks) {
    // Loop through all the particles in the block
    for (Particle & particle : block.particles) {
      // Cast from double to float and write to file
      particles_out.push_back(particle);
    }
  }
  // Sort particles_out by id
  sort(particles_out.begin(), particles_out.end(), [](Particle & a, Particle & b) { return a.id < b.id; });
  for (Particle & particle : particles_out) {
    cout << "Id: " << particle.id << std::endl;
    cout << "Pos: " << particle.p[0] << " " << particle.p[1] << " " << particle.p[2] << std::endl;
    cout << "Hv: " << particle.hv[0] << " " << particle.hv[1] << " " << particle.hv[2] << std::endl;
    cout << "Vel: " << particle.v[0] << " " << particle.v[1] << " " << particle.v[2] << std::endl;
    cout << "Acc: " << particle.a[0] << " " << particle.a[1] << " " << particle.a[2] << std::endl;
    cout << "Density: " << particle.rho << std::endl;
    cout << "Block: " << particle.current_block << std::endl;
    cout << "--------------------"<< std::endl;
  }

  return particles_out;
}
