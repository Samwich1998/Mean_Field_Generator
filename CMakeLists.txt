# CMakeLists for the temp analysis repository

# Set up
cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

# ROOT
find_package(ROOT REQUIRED)

# Project name
project (Mean_Project)

set(INCLUDES ${ROOT_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)
include_directories(${INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

# Add executable for generator
add_executable(gen_root MF_generator.cpp spectral_interpolation.cpp Cross_Sections.cpp helpers.cpp)
target_link_libraries(gen_root ${ROOT_LIBRARIES})

# make his
add_executable(his_yay histograms.cpp)
target_link_libraries(his_yay ${ROOT_LIBRARIES})

