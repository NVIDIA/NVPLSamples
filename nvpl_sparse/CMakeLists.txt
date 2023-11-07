cmake_minimum_required(VERSION 3.12)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type: Release, Debug, etc" FORCE)
endif()

project(nvpl_sparse_examples LANGUAGES C)
set(CMAKE_C_STANDARD 99)

find_package(OpenMP REQUIRED COMPONENTS C)

find_package(nvpl REQUIRED COMPONENTS sparse)

enable_testing()
set(examples_install_prefix "examples")

add_subdirectory(c)
