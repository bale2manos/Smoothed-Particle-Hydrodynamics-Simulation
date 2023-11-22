#!/bin/bash
. /etc/profile
module avail
module load gcc/12.1.0
cd ..
cmake -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
BUILD_DIR="build-debug"
"$BUILD_DIR/fluid/fluid" 10 "./in/large.fld" "../outputftestdebug10.fld"