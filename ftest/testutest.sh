#!/bin/bash
BUILD_DIR="../cmake-build-debug"
mkdir "$BUILD_DIR"
# Change to the build directory
cd "$BUILD_DIR"
# Run CMake to configure the build
cmake ..
make
"$BUILD_DIR/utest/utest"