//
// Created by carlos on 21/11/23.
//

#include <gtest/gtest.h>

/**
 * @brief Entry point for running all the tests.
 *
 * This function initializes Google Test and runs all the test cases.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return The result of running all tests.
 */

int main(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}