cmake_minimum_required(VERSION 3.12)

project("nvpl_tensor_version"
        LANGUAGES C CXX
        DESCRIPTION "NVPL Tensor example: Print version and exit")

include(GNUInstallDirs)
find_package(nvpl REQUIRED COMPONENTS tensor)

# C Example
add_executable(nvpl_tensor_version_c nvpl_tensor_version.c)
target_link_libraries(nvpl_tensor_version_c PUBLIC nvpl::tensor)

# CXX Example
add_executable(nvpl_tensor_version_cxx nvpl_tensor_version.cxx)
target_link_libraries(nvpl_tensor_version_cxx PUBLIC nvpl::tensor)

set(ALL_TARGETS nvpl_tensor_version_c nvpl_tensor_version_cxx)

# Install targets
install(TARGETS ${ALL_TARGETS} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# CTest
enable_testing()
foreach(tgt IN LISTS ALL_TARGETS)
  add_test(${tgt} ${tgt})
endforeach()
