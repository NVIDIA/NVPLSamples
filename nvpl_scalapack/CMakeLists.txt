# NVPL ScaLAPACK: Example CMake Project
#
# Minimal examples building against any combination of interfaces
# as determined by CMake variables:
# * INT_TYPE:
#    - 'lp64'
#    - 'ilp64'
# * THR_TYPE: Threading type for LAPACK/BLAS.  One of:
#   - 'seq' : sequential
#   - 'omp' : OpenMP
# * MPI_INTERFACE: BLACS MPI interface:
#   - 'openmpi3' : OpenMPI-3.x
#   - 'openmpi4' : OpenMPI-4.x
#   - 'openmpi5' : OpenMPI-5.x
#   - 'mpich' : Any MPICH-ABI compatible MPI distribution
#
# Use requires MPI.
#  * See: https://cmake.org/cmake/help/latest/module/FindMPI.html
#  * To detect MPI
#    * Set Environment variable `MPIEXEC_EXECUTABLE` to full path to mpiexec
#    * [OR] Set Environment variable `MPI_HOME` to the base directory of MPI installation.
#  * Ensure the MPI_INTERFACE is set to match your detected MPI distribution
#
# This project has CMake Presets enabled for >=CMake-3.20 for each combination of
# int, thread.
# CMake>=3.23:
#   Use:  `cmake --list-presets`  to list available presets
#   Configure:  `cmake --preset <preset>`
#   Build: `cmake --build --preset <preset>`
# CMake<3.23:
#   Configure:  `cmake -S . -B build -DINT_TYPE=lp64  -DTHR_TYPE=seq -DMPI_INTERFACE=openmpi4`
#   Build: `cmake --build build`
#

cmake_minimum_required(VERSION 3.12)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type: Release, Debug, etc")
endif()

project(nvpl_scalapack_examples LANGUAGES Fortran C CXX)

include(CTest)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 14)

## Configuration options
# Set to control wich NVPL BLAS library variant to link against
set(INT_TYPE "lp64" CACHE STRING
    "NVPL Fortran integer type. Valid options: lp64, ilp64")
set(THR_TYPE "seq" CACHE STRING
    "NVPL LAPACK and BLAS threading runtime. Valid options: seq, gomp, lomp")
set(MPI_INTERFACE "openmpi4" CACHE STRING
    "NVPL BLACS MPI interface. Valid options: openmpi3 openmpi4, openmpi5, mpich")
set(MPI_NUMPROC "4" CACHE STRING
    "MPI NUMPROC to run examples with")
set(OMP_NUM_THREADS "4" CACHE STRING
    "OMP_NUM_THREADS to run examples with")

## Find MPI
# To Find the correct MPI distribution:
# * Ensure that the correct "mpiexec" is on the PATH via OS MPI selection mechanism.
# * If the system `mpiexec` has a different name set MPIEXEC_EXECUTABLE to the full path to executable
# * Ensure CC, CXX, F77 and FC variables are unset or set to normal compilers,
#   and not the mpi compiler wrappers.  Forcing the compiler wrappers can be problematic.
if(NOT EXISTS ${MPIEXEC_EXECUTABLE})
    set(MPIEXEC_EXECUTABLE $ENV{MPIEXEC_EXECUTABLE})
    if(NOT EXISTS ${MPIEXEC_EXECUTABLE})
        find_program(MPIEXEC_EXECUTABLE mpiexec
            HINTS ${MPI_HOME} ENV MPI_HOME
            PATHS /usr /usr/local
        )
    endif()
endif()
find_package(MPI REQUIRED Fortran C)
if(NOT MPIEXEC_EXECUTABLE)
  set(MPIEXEC_EXECUTABLE "mpiexec" CACHE STRING "mpiexec executable" FORCE)
  message(STATUS "MPIEXEC_EXECUTABLE not set.  Default: MPIEXEC_EXECUTABLE=${MPIEXEC_EXECUTABLE}")
else()
  message(STATUS "MPIEXEC_EXECUTABLE: ${MPIEXEC_EXECUTABLE}")
endif()

# Find NVPL ScaLAPACK
find_package(nvpl REQUIRED COMPONENTS scalapack)

# Default test properties
set(TEST_TIMEOUT_SECONDS 10)

list(APPEND TEST_PROPERTIES ENVIRONMENT OMP_NUM_THREADS=${OMP_NUM_THREADS})
list(APPEND TEST_PROPERTIES TIMEOUT ${TEST_TIMEOUT_SECONDS})
set(EXAMPLE_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})

add_subdirectory(cxx)
add_subdirectory(f77)
