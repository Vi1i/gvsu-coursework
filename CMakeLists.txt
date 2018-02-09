cmake_minimum_required (VERSION 3.1 FATAL_ERROR)
project (nbdc)
set(CMAKE_CXX_STANDARD 11)

set(nbdc_VERSION_MAJOR 0)
set(nbdc_VERSION_MINOR 1)
set(nbdc_VERSION_PATCH 0)

configure_file (
  "${PROJECT_SOURCE_DIR}/nbdc_config.hpp.in"
  "${PROJECT_BINARY_DIR}/nbdc_config.hpp"
)
include_directories(${PROJECT_BINARY_DIR})
################################################################################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH}
)

# Note this is the 'bad' way of doing this, but the recommended alternative is to keep a list of *all* source files =/
file(GLOB_RECURSE nbdc_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")
include_directories(${CMAKE_SOURCE_DIR}/src)

# Final target
add_executable(nbdc ${nbdc_SRC})
#target_link_libraries(nbdc)
