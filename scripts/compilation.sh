#!/bin/sh
. /etc/profile
module avail
module load gcc/12.1.0
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release