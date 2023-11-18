enable_testing()

add_executable(ftest
        fluid_test.cpp
        fluid_test.hpp

)

target_link_libraries(
        ftest PRIVATE
        sim
        GTest::gtest_main
        Microsoft.GSL::GSL
)

include(GoogleTest)
gtest_discover_tests(ftest)