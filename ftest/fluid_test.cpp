//
// Created by carlos on 6/11/23.
//
#include "fluid_test.hpp"

#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

TEST(CheckOutSmall, Test1) {
  std::filesystem::path const currentPath = std::filesystem::current_path();
  std::string const pathsmall             = currentPath.string() + "/testfiles/small.fld";
  std::string const pathfile              = currentPath.string() + "/";
  std::cout << pathfile;
  Malla malla     = read_input_file(pathsmall.c_str());
  int const tests = 5;
  for (int iter = 1; iter <= tests; iter++) {
    malla.mallaInteraction(1);
    Malla const small =
        read_input_file((pathfile + "out/small-" + std::to_string(iter) + ".fld").c_str());
    EXPECT_EQ(comparevectorsout(malla.getParticles(), small.getParticles()), 0);
  }
}

TEST(CheckOutLarge, Test2) {
  std::filesystem::path const currentPath = std::filesystem::current_path();
  std::string const pathlarge             = currentPath.string() + "/testfiles/large.fld";
  std::string const pathfile              = currentPath.string() + "/";
  Malla malla                             = read_input_file(pathlarge.c_str());
  int const tests                         = 5;
  for (int iter = 1; iter <= tests; iter++) {
    malla.mallaInteraction(1);
    std::string const pathlarge2 = pathfile + "out/large-" + std::to_string(iter) + ".fld";
    Malla const large            = read_input_file(pathlarge2.c_str());
    EXPECT_EQ(comparevectorsout(malla.getParticles(), large.getParticles()), 0);
  }
}

TEST(CheckOutSmallTrz, Test3) {
  std::filesystem::path const currentPath = std::filesystem::current_path();
  std::string const pathsmall             = currentPath.string() + "/testfiles/small.fld";
  std::string const pathfile              = currentPath.string() + "/";
  Malla malla                             = read_input_file(pathsmall.c_str());
  int const tests                         = 5;
  for (int iter = 1; iter <= tests; iter++) {
    malla.mallaInteraction(1);
    std::string const pathtrz =
        pathfile + "trz/small/boundint-base-" + std::to_string(iter) + ".trz";
    EXPECT_EQ(compare_iteration(malla, pathtrz.c_str()), 0);
  }
}

TEST(CheckOutLargeTrz, Test4) {
  std::filesystem::path const currentPath = std::filesystem::current_path();
  std::string const pathlarge             = currentPath.string() + "/testfiles/large.fld";
  std::string const pathfile              = currentPath.string() + "/";
  Malla malla                             = read_input_file(pathlarge.c_str());
  int const tests                         = 5;
  for (int iter = 1; iter <= tests; iter++) {
    malla.mallaInteraction(1);
    std::string const pathtrz =
        pathfile + "trz/large/boundint-base-" + std::to_string(iter) + ".trz";
    EXPECT_EQ(compare_iteration(malla, pathtrz.c_str()), 0);
  }
}

int compare_iteration(Malla & malla, char const * inputFileName) {
  std::vector<Particle> particles_out1;
  std::vector<Particle> particles_out2;
  particles_out2 = traceread(inputFileName);
  particles_out1 = malla.getParticles();
  comparevectorstrz(particles_out1, particles_out2);
  return 0;
}

int comparevectorstrz(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2) {
  sort(particles_out1.begin(), particles_out1.end(), [](Particle & a, Particle & b) {
    return a.id < b.id;
  });
  sort(particles_out2.begin(), particles_out2.end(), [](Particle & a, Particle & b) {
    return a.id < b.id;
  });

  if (particles_out1.size() != particles_out2.size()) { return -1; }
  return compare(particles_out1, particles_out2);
}

int compare(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2) {
  for (int iter = 0; iter < int(particles_out1.size()); iter++) {
    if (particles_out1[iter].id != particles_out2[iter].id ||
        particles_out1[iter].current_block != particles_out2[iter].current_block ||
        particles_out1[iter].current_block != particles_out2[iter].current_block ||
        particles_out1[iter].density != particles_out2[iter].density ||
        particles_out1[iter].pos[0] != particles_out2[iter].pos[0] ||
        particles_out1[iter].pos[1] != particles_out2[iter].pos[1] ||
        particles_out1[iter].pos[2] != particles_out2[iter].pos[2] ||
        particles_out1[iter].h_vel[0] != particles_out2[iter].h_vel[0] ||
        particles_out1[iter].h_vel[1] != particles_out2[iter].h_vel[1] ||
        particles_out1[iter].h_vel[2] != particles_out2[iter].h_vel[2] ||
        particles_out1[iter].vel[0] != particles_out2[iter].vel[0] ||
        particles_out1[iter].vel[1] != particles_out2[iter].vel[1] ||
        particles_out1[iter].vel[2] != particles_out2[iter].vel[2] ||
        particles_out1[iter].acc[0] != particles_out2[iter].acc[0] ||
        particles_out1[iter].acc[1] != particles_out2[iter].acc[1] ||
        particles_out1[iter].acc[2] != particles_out2[iter].acc[2]) {
      return -2;
    }
  }
  return 0;
}

int comparevectorsout(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2) {
  sort(particles_out1.begin(), particles_out1.end(), [](Particle & a, Particle & b) {
    return a.id < b.id;
  });
  sort(particles_out2.begin(), particles_out2.end(), [](Particle & a, Particle & b) {
    return a.id < b.id;
  });
  if (particles_out1.size() != particles_out2.size()) { return -1; }

  return comparetrzvec(particles_out1, particles_out2);
}

int comparetrzvec(std::vector<Particle> particles_out1, std::vector<Particle> particles_out2) {
  double const delta = exp10(-3);
  for (int iter = 0; iter < int(particles_out1.size()); iter++) {
    if (particles_out1[iter].id != particles_out2[iter].id ||
        particles_out1[iter].current_block != particles_out2[iter].current_block ||
        std::abs(particles_out1[iter].pos[0] - particles_out2[iter].pos[0]) > delta ||
        std::abs(particles_out1[iter].pos[1] - particles_out2[iter].pos[1]) > delta ||
        std::abs(particles_out1[iter].pos[2] - particles_out2[iter].pos[2]) > delta ||
        std::abs(particles_out1[iter].h_vel[0] - particles_out2[iter].h_vel[0]) > delta ||
        std::abs(particles_out1[iter].h_vel[1] - particles_out2[iter].h_vel[1]) > delta ||
        std::abs(particles_out1[iter].h_vel[2] - particles_out2[iter].h_vel[2]) > delta ||
        std::abs(particles_out1[iter].vel[0] - particles_out2[iter].vel[0]) > delta ||
        std::abs(particles_out1[iter].vel[1] - particles_out2[iter].vel[1]) > delta ||
        std::abs(particles_out1[iter].vel[2] - particles_out2[iter].vel[2]) > delta) {
      std::cout << particles_out1[iter].id << '\n';
      std::cout << particles_out2[iter].id << '\n';
      return -2;
    }
  }
  return 0;
}

std::vector<Particle> traceread(char const * traceFileName) {
  std::ifstream traceFile(traceFileName, std::ios::binary);
  int numBlocks = 0;
  read_field_from_file_int(traceFile, numBlocks);
  std::vector<Particle> particles_out;
  for (int iter = 0; iter < numBlocks; iter++) {
    TraceBlock block;
    read_field_from_file_long(traceFile, block.numParticles);
    for (long j = 0; j < block.numParticles; j++) {
      Particle const particle = makeparticle(traceFile, iter);
      particles_out.push_back(particle);
    }
  }

  return particles_out;
}

Particle makeparticle(std::ifstream & traceFile, int iter) {
  Particle particle{};
  read_field_from_file_long(traceFile, particle.id);
  read_field_from_file_double(traceFile, particle.pos[0]);
  read_field_from_file_double(traceFile, particle.pos[1]);
  read_field_from_file_double(traceFile, particle.pos[2]);
  read_field_from_file_double(traceFile, particle.h_vel[0]);
  read_field_from_file_double(traceFile, particle.h_vel[1]);
  read_field_from_file_double(traceFile, particle.h_vel[2]);
  read_field_from_file_double(traceFile, particle.vel[0]);
  read_field_from_file_double(traceFile, particle.vel[1]);
  read_field_from_file_double(traceFile, particle.vel[2]);
  read_field_from_file_double(traceFile, particle.density);
  read_field_from_file_double(traceFile, particle.acc[0]);
  read_field_from_file_double(traceFile, particle.acc[1]);
  read_field_from_file_double(traceFile, particle.acc[2]);
  particle.current_block = iter;
  return particle;
}

void read_field_from_file_double(std::ifstream & input_file, auto & field_to_read) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input_file.read(reinterpret_cast<char *>(&field_to_read), sizeof(double));
}

void read_field_from_file_long(std::ifstream & input_file, auto & field_to_read) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input_file.read(reinterpret_cast<char *>(&field_to_read), sizeof(long));
}

void read_field_from_file_int(std::ifstream & input_file, auto & field_to_read) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input_file.read(reinterpret_cast<char *>(&field_to_read), sizeof(int));
}