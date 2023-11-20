//
// Created by bale2 on 26/09/2023.
//
#include "sim/block.hpp"

#include <gtest/gtest.h>

// Caso correcto

TEST(BlockTest, InitialBlockIndex) {
  EXPECT_EQ(initial_block_index(12, 3, 4), 2);
}

// Caso correcto
TEST(BlockTest, CalculateBlockIndex) {
  std::array<int, 3> const miVector = {1, 1, 1};
  EXPECT_EQ(calculate_block_index(miVector, 5, 12), 66);
}

TEST(BlockTest, CheckNeighboursEsquinas) {
  std::array<int, 3> const coords      = {0, 0, 0};
  std::array<int, 3> const dimensiones = {5, 5, 5};
  std::vector<int> const indexvecinos  = {0, 1, 5, 6, 25, 26, 30, 31};
  EXPECT_EQ(check_neighbours(coords, dimensiones), indexvecinos);
}

TEST(BlockTest, CheckNeighbourscentral) {
  std::array<int, 3> const coords      = {3, 3, 3};
  std::array<int, 3> const dimensiones = {5, 5, 5};
  std::vector<int> const indexvecinos  = {93,  62,  63,  64,  67,  68,  69,  72,  73,
                                          74,  87,  88,  89,  92,  94,  97,  98,  99,
                                          112, 113, 114, 117, 118, 119, 122, 123, 124};
  EXPECT_EQ(check_neighbours(coords, dimensiones), indexvecinos);
}

TEST(BlockTest, CheckNeighbourspared) {
  std::array<int, 3> const coords      = {3, 3, 0};
  std::array<int, 3> const dimensiones = {5, 5, 5};
  std::vector<int> const indexvecinos  = {18, 12, 13, 14, 17, 19, 22, 23, 24,
                                          37, 38, 39, 42, 43, 44, 47, 48, 49};
  EXPECT_EQ(check_neighbours(coords, dimensiones), indexvecinos);
}

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}