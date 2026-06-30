cmake_minimum_required (VERSION 3.1 FATAL_ERROR)
project (flesch-index)
set (CMAKE_CXX_STANDARD 11)

set(flesch_index_VERSION_MAJOR 0)
set(flesch_index_VERSION_MINOR 1)
set(flesch_index_VERSION_PATCH 0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/include/flesch-index_config.h.in"
  "${PROJECT_BINARY_DIR}/include/flesch-index_config.h"
)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_BINARY_DIR}/include")

add_executable(flesch-index src/main.cxx src/flesch-index.cxx)
