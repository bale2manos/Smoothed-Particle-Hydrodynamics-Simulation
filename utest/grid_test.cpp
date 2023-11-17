#include <gtest/gtest.h>
#include "sim/grid.hpp"

Malla grid(15138, 292);

TEST(GridTest, smoothlenght){
  double result = grid.smooth_length_calc();
  EXPECT_EQ(result,0.0058047945205479453);
}

TEST(GridTest, nxcalc){
  EXPECT_EQ(grid.nx_calc(84,25,20),2);
}

TEST(GridTest, nycalc){
  EXPECT_EQ(grid.ny_calc(50,25,20),1);
}

TEST(GridTest, nzcalc){
  EXPECT_EQ(grid.nx_calc(401,100,20),15);
}

TEST(GridTest, sx_calc){
  EXPECT_EQ(grid.sx_calc(100,50,20),2.5);
}

TEST(GridTest, sy_calc){
  EXPECT_EQ(grid.sy_calc(200,50,20),7.5);
}

TEST(GridTest, sz_calc){
  EXPECT_EQ(grid.sz_calc(1000,350,10),65);
}

TEST(GridTest, particle_mass){
  EXPECT_EQ(grid.particle_mass_calc(),4.0165339818054222e-05);
}


TEST(GridTest, createFillGridsize) {
  vector<Block> blocks = grid.createFillGrid();
  EXPECT_EQ(blocks.size(), 30008);
}


TEST(GridTest, density_transformation){
  EXPECT_EQ(grid.density_transformation(1000), 8.4091188953149706e+18);
}


TEST(GridTest, increase_density_noincrease){
  array<double, 3> pivot_coords = {2.0, 3.0, 4.0};
  array<double, 3> particle2_coords = {3.0, 1.0, 3.0};
  double pivot_rho_ori = 1.0;
  double particle2_rho_ori = 2.0;
  double pivot_rho = 1.0;
  double particle2_rho = 2.0;
  grid.increase_density(pivot_coords, particle2_coords, pivot_rho, particle2_rho);
  EXPECT_EQ(pivot_rho, pivot_rho_ori);  // Los valores son iguales porque norm_squared > h_squared
  EXPECT_EQ(particle2_rho, particle2_rho_ori);
}

TEST(GridTest, increase_density_increase){
  array<double, 3> pivot_coords = {0.0005, 0.00004, 0.0003};
  array<double, 3> particle2_coords = {0.00049, 0.00004, 0.00002};
  double pivot_rho_ori = 1.0;
  double particle2_rho_ori = 2.0;
  double pivot_rho = 1.0;
  double particle2_rho = 2.0;
  grid.increase_density(pivot_coords, particle2_coords, pivot_rho, particle2_rho);
  EXPECT_GT(pivot_rho, pivot_rho_ori);  // Los valores NO son iguales porque norm_squared < h_squared y por tanto hay incremento
  EXPECT_GT(particle2_rho, particle2_rho_ori);
}

TEST(GridTest, particle_movement){
  Particle particle;
  Particle particle2;
  particle.h_vel  = {4.1, 5.4, 6.4};
  particle.acc  = {2.4, 3.5, 1.1};
  particle.pos  = {2.3, 2.1, 3.1};
  particle2.h_vel = {4.1, 5.4, 6.4};
  particle2.acc = {2.4, 3.5, 1.1};
  particle2.pos = {2.3, 2.1, 3.1};
  grid.particle_movement(particle);
  EXPECT_NE(particle.pos[1], particle2.pos[1]);
  EXPECT_NE(particle.vel[1], particle2.vel[1]);
  EXPECT_NE(particle.h_vel[1], particle2.h_vel[1]);
}

/*
Faltan las colisiones!!!!!!!
TEST(GridTest, AccelerationTransferTest) {
  // Crea instancias de la clase Malla y las partículas (ajusta según sea necesario
  Particle pivot;
  Particle pivot2;
  Particle particle2;

  // Inicializa las posiciones, densidades y velocidades de las partículas (ajusta según sea necesario)
  pivot.p = {1, 2, 3};
  pivot.density = 1.0;
  pivot.v = {1, 3, 2};
  pivot2.p = {1, 2, 3};
  pivot2.density = 1.0;
  pivot2.v = {1, 3, 2};
  particle2.p = {1.0, 1.0, 1.0};
  particle2.density = 2.0;
  particle2.v = {1.0, 1.0, 1.0};

  grid.acceleration_transfer_calculations(pivot, particle2);

  EXPECT_NE(pivot.a[2], pivot2.a[2]);  // Los valores son iguales porque norm_squared > h_squared
}

*/

