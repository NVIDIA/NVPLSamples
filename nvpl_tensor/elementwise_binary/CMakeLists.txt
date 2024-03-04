cmake_minimum_required(VERSION 3.12)

project("nvpl_tensor_elementwise_binary"
        LANGUAGES C
        DESCRIPTION "NVPL Tensor example: Elementwise Binary")

include(GNUInstallDirs)
find_package(nvpl REQUIRED COMPONENTS tensor)

# C Example
add_executable(nvpl_tensor_elementwise_binary_c elementwise_binary.c)
target_link_libraries(nvpl_tensor_elementwise_binary_c PUBLIC nvpl::tensor)

set(ALL_TARGETS nvpl_tensor_elementwise_binary_c)

# Install targets
install(TARGETS ${ALL_TARGETS} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# CTest
enable_testing()
foreach(tgt IN LISTS ALL_TARGETS)
  add_test(${tgt} ${tgt})
endforeach()
