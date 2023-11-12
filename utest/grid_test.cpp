#include <gtest/gtest.h>
#include "sim/grid.hpp"

Malla grid(15138, 292);

TEST(GridTest, smoothlenght){
  double result = grid.smooth_length();
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
  EXPECT_EQ(grid.particle_mass(),4.0165339818054222e-05);
}


TEST(GridTest, createFillGridsize) {
  vector<Block> blocks = grid.createFillGrid();
  EXPECT_EQ(blocks.size(), 30008);
}


TEST(GridTest, density_transformation){
  EXPECT_EQ(grid.density_transformation(1000), 8.4091188953149706e+18);
}

TEST(GridTest, increase_density){
  EXPECT_EQ(grid.increase_density({1,2,3},{2,3,6},1000,900), 0.055947441747640575);
}

/*

TEST(GridTest, acceleration_transfer){
  Particle p1;
  p1.p = {1.0, 2.0, 3.0};
  p1.hv = {0.1, 0.2, 0.3};
  p1.rho = 1000;
  p1.id = 1;
  p1.current_block = 2;

  Particle p2;
  p2.p = {2, 3, 6};
  p2.hv = {2, 3, 4};
  p2.rho = 900;
  p2.id = 2;
  p2.current_block = 3;

  std::array<double, 2> acc_trans = {6, 3};
  std::array<double, 3> acc_increase = { 0.055947441747640575, 0.055947441747640575, 0.16784232524292172 };

  EXPECT_EQ(acceleration_transfer(p1,p2,20,acc_trans),acc_increase);
}

*/

