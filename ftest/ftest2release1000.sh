#!/bin/bash
cd ..
cmake -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
BUILD_DIR="build-release"
start_time=$(date +%s.%N)
"$BUILD_DIR/fluid/fluid" 1000 "./in/large.fld" "../outputrelease1000.fld"
end_time=$(date +%s.%N)
elapsed_time=$(echo "$end_time - $start_time" | bc)
echo "Total execution time: $elapsed_time seconds"