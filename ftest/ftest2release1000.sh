#!/bin/bash
# Create a build directory (if it doesn't exist)
BUILD_DIR="../cmake-build-release"
mkdir -p "$BUILD_DIR"
# Change to the build directory
cd "$BUILD_DIR"
# Run CMake to configure the build
cmake ..
make
start_time=$(date +%s.%N)
"$BUILD_DIR/fluid/fluid" 1000 "large.fld" "../outputrelease1000.fld"
end_time=$(date +%s.%N)
elapsed_time=$(echo "$end_time - $start_time" | bc)
echo "Total execution time: $elapsed_time seconds"