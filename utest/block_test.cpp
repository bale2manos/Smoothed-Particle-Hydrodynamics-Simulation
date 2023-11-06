//
// Created by bale2 on 26/09/2023.
//
#include <gtest/gtest.h>
#include "sim/block.hpp"

// Caso correcto

TEST(BlockTest, InitialBlockIndex) {
  EXPECT_EQ(initial_block_index(12,3,4), 2);
}
// Caso index division 0 ¿?

TEST(BlockTest, InitialBlockIndexdiv0) {
  ASSERT_THROW(initial_block_index(12,3,0), std::invalid_argument);
}

// Caso correcto
TEST(BlockTest, CalculateBlockIndex) {
  std::array<int, 3> miVector = {1, 1, 1};
  EXPECT_EQ(calculate_block_index(miVector,5,12), 66);
}
// Caso index negativo ¿?
TEST(BlockTest, CalculateBlockIndexnegative)  {
  std::array<int, 3> miVector = {1, 1, 1};
  EXPECT_EQ(calculate_block_index(miVector,-5,12), -64);
}

TEST(BlockTest, CheckNeighboursEsquinas) {
  std::array<int, 3> coords = {0, 0, 0};
  std::array<int, 3> dimensiones = {5,5,5};
  std::vector<int> indexvecinos = {31, 6, 26, 1, 30, 5, 25, 0};
  EXPECT_EQ(check_neighbours(coords,dimensiones), indexvecinos);
}

TEST(BlockTest, CheckNeighbourscentral) {
  std::array<int, 3> coords = {3,3,3};
  std::array<int, 3> dimensiones = {5,5,5};
  std::vector<int> indexvecinos = {124, 99, 74, 119, 94, 69, 114, 89, 64, 123, 98, 73, 118, 93, 68, 113, 88, 63, 122, 97, 72, 117, 92, 67, 112, 87, 62};
  EXPECT_EQ(check_neighbours(coords,dimensiones), indexvecinos);
}

TEST(BlockTest, CheckNeighbourspared) {
  std::array<int, 3> coords = {3,3,0};
  std::array<int, 3> dimensiones = {5,5,5};
  std::vector<int> indexvecinos = { 49, 24, 44, 19, 39, 14, 48, 23, 43, 18, 38, 13, 47, 22, 42, 17, 37, 12 };
  EXPECT_EQ(check_neighbours(coords,dimensiones), indexvecinos);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}