add_executable(fluid fluid.cpp)
target_link_libraries ( fluid sim)
add_custom_command(
        TARGET fluid POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/fluid/small.fld
        ${CMAKE_CURRENT_BINARY_DIR}/small.fld
)