#!/bin/bash
cd ..
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
BUILD_DIR="build-debug"
"$BUILD_DIR/utest/utest"