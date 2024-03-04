cmake_minimum_required(VERSION 3.12)

project("nvpl_tensor_permutation"
        LANGUAGES C
        DESCRIPTION "NVPL Tensor example: Permutation")

include(GNUInstallDirs)
find_package(nvpl REQUIRED COMPONENTS tensor)

# C Example
add_executable(nvpl_tensor_permutation_c permutation.c)
target_link_libraries(nvpl_tensor_permutation_c PUBLIC nvpl::tensor)

set(ALL_TARGETS nvpl_tensor_permutation_c)

# Install targets
install(TARGETS ${ALL_TARGETS} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# CTest
enable_testing()
foreach(tgt IN LISTS ALL_TARGETS)
  add_test(${tgt} ${tgt})
endforeach()
