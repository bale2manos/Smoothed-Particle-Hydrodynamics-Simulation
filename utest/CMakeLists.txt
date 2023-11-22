enable_testing()

add_executable(utest
        block_test
        block_test.cpp
        block_test.hpp
        progargs_test
        progargs_test.cpp
        progargs_test.hpp
        grid_test
        grid_test.cpp
        grid_test.hpp
        fluid_test
        fluid_test.cpp
        fluid_test.hpp
        main.cpp
        ../cmake-build-release/utest/utest

)

target_link_libraries(
        utest PRIVATE
        sim
        GTest::gtest_main
        Microsoft.GSL::GSL
)

include(GoogleTest)
gtest_discover_tests(utest)
