# NVPL FFT: Examples
cmake_minimum_required(VERSION 3.12)
project(nvpl_fft_examples DESCRIPTION "NVPL FFT: Examples" LANGUAGES CXX)

include(CTest)
include(CheckLanguage)
include(CMakeDetermineFortranCompiler)

## Options
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type: Release, Debug, etc" FORCE)
endif()

set(EXAMPLE_INSTALL_DIR test/nvpl_fft/examples CACHE STRING "Examples binary install dir" FORCE)

## CXX Language standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Dependencies
find_package(OpenMP REQUIRED COMPONENTS CXX)
find_package(nvpl REQUIRED COMPONENTS fft)

## CXX Examples
add_subdirectory(cxx)

## Fortran examples
check_language(Fortran)
if ("${CMAKE_Fortran_COMPILER_ID}" STREQUAL "GNU")
    enable_language(Fortran)
    add_subdirectory(fortran)
else()
  message(STATUS "GNU Fortran compiler could not be found")
endif()
