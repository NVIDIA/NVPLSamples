# NVPL LAPACK: Examples
#
cmake_minimum_required(VERSION 3.12)
project(
  nvpl_lapack_examples
  LANGUAGES Fortran C CXX
)

# Find NVPL LAPACK
find_package(OpenMP REQUIRED COMPONENTS CXX Fortran)
find_package(nvpl REQUIRED COMPONENTS lapack)

# Enable testing at project root level
enable_testing()

# NVPL LAPACK: Subdirectories
add_subdirectory(f77)
add_subdirectory(cpp)

