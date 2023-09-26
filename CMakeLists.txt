cmake_minimum_required(VERSION 3.26)
project(Arquitectura_de_Computadores)

set(CMAKE_CXX_STANDARD 17)

add_executable(Arquitectura_de_Computadores fluid/fluid.cpp
        sim/grid.hpp
        sim/grid.cpp
        sim/progargs.cpp
        sim/progargs.hpp
        sim/block.cpp
        sim/block.hpp
        utest/progargs_test.cpp
        utest/progargs_test.hpp
        utest/grid_test.cpp
        utest/grid_test.hpp
        utest/block_test.cpp
        utest/block_test.hpp)
