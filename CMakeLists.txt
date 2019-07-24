cmake_minimum_required (VERSION 2.6)

set(CMAKE_PROJECT_VERSION 0)
set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 1)
set(CMAKE_PROJECT_VERSION_PATCH 0)

project (F1_2018_Telemetry)

# The version number.
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}/external")
add_subdirectory(external/json)
add_subdirectory(external/aws-sdk-cpp)
add_subdirectory(src/)
# add the executable
#add_executable (Tutorial tutorial.cxx)
#target_link_libraries( MathFunctions)
add_executable(f1 src/main.cpp)