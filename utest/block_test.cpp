//
// Created by bale2 on 26/09/2023.
//
#include "sim/block.hpp"

#include <gtest/gtest.h>

// Caso correcto

/**
 * @brief Test for the initial block index calculation.
 *
 * @param BlockTest Fixture for block-related tests.
 */

TEST(BlockTest, InitialBlockIndex) {
  EXPECT_EQ(initial_block_index(12, 3, 4), 2);
}

/**
 * @brief Test for calculating the block index with a valid case.
 *
 * This test case uses a 3D vector and checks if the block index is calculated correctly.
 *
 * @param BlockTest Fixture for block-related tests.
 */

TEST(BlockTest, CalculateBlockIndex) {
  std::array<int, 3> const miVector = {1, 1, 1};
  EXPECT_EQ(calculate_block_index(miVector, 5, 12), 66);
}

/**
 * @brief Test for checking neighbors in corner cases.
 *
 * This test case creates a block with coordinates at the corner of the grid and checks
 * if the neighbors are correctly identified.
 *
 * @param BlockTest Fixture for block-related tests.
 */

TEST(BlockTest, CheckNeighboursEsquinas) {
  std::array<int, 3> const coords      = {0, 0, 0};
  std::array<int, 3> const dimensiones = {5, 5, 5};
  std::vector<int> const indexvecinos  = {0, 1, 5, 6, 25, 26, 30, 31};
  Block const bloque(coords, dimensiones);
  EXPECT_EQ(bloque.neighbours, indexvecinos);
}

/**
 * @brief Test for checking neighbors in the central case.
 *
 * This test case creates a block with coordinates at the center of the grid and checks
 * if the neighbors are correctly identified.
 *
 * @param BlockTest Fixture for block-related tests.
 */

TEST(BlockTest, CheckNeighbourscentral) {
  std::array<int, 3> const coords      = {3, 3, 3};
  std::array<int, 3> const dimensiones = {5, 5, 5};
  std::vector<int> const indexvecinos  = {62,  63,  64,  67,  68,  69,  72,  73,  74,
                                          87,  88,  89,  92,  93,  94,  97,  98,  99,
                                          112, 113, 114, 117, 118, 119, 122, 123, 124};
  Block const bloque(coords, dimensiones);
  EXPECT_EQ(bloque.neighbours, indexvecinos);
}

/**
 * @brief Test for checking neighbors in a specific case.
 *
 * This test case creates a block with coordinates at a specific location and checks
 * if the neighbors are correctly identified.
 *
 * @param BlockTest Fixture for block-related tests.
 */

TEST(BlockTest, CheckNeighbourspared) {
  std::array<int, 3> const coords      = {3, 3, 0};
  std::array<int, 3> const dimensiones = {5, 5, 5};
  std::vector<int> const indexvecinos  = {12, 13, 14, 17, 18, 19, 22, 23, 24,
                                          37, 38, 39, 42, 43, 44, 47, 48, 49};
  Block const bloque(coords, dimensiones);
  EXPECT_EQ(bloque.neighbours, indexvecinos);
}

