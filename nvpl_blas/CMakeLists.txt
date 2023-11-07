# NVPL BLAS: Example CMake Project
#
# Minimal examples building Fortran and C examples against any combination of interfaces
# as determined by CMake variables:
# * INT_TYPE: 'lp64' or 'ilp64
# * THR_TYPE: 'seq' or 'omp'
#
# This project has CMake Presets enabled for >=CMake-3.20 for each combination of int
# and thread type.   Older CMake versions should set -DINT_TYPE= and -DTHR_TYPE with the
# CMake configure command.
#

cmake_minimum_required(VERSION 3.12)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type: Release, Debug, etc")
endif()

project(nvpl_blas_examples LANGUAGES Fortran C)
set(CMAKE_C_STANDARD 99)

## Configuration options
# Set to control wich NVPL BLAS library variant to link against
set(INT_TYPE "lp64" CACHE STRING
    "NVPL BLAS integer type. Valid options: lp64, ilp64")
set(THR_TYPE "omp" CACHE STRING
    "NVPL BLAS threading runtime. Valid options: seq, omp")

if (THR_TYPE STREQUAL "omp")
    find_package(OpenMP REQUIRED COMPONENTS Fortran C)
endif()

# Find NVPL BLAS
find_package(nvpl REQUIRED COMPONENTS blas)

enable_testing()
set(examples_install_prefix "examples")

add_subdirectory(c)
add_subdirectory(f77)
