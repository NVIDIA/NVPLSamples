cmake_minimum_required(VERSION 3.12)

project("nvpl_tensor_examples"
        LANGUAGES C CXX
        DESCRIPTION "NVPL Tensor examples")

include(GNUInstallDirs)
find_package(nvpl REQUIRED COMPONENTS tensor)
enable_testing()

add_subdirectory(contraction)
add_subdirectory(elementwise_binary)
add_subdirectory(elementwise_trinary)
add_subdirectory(nvpl_tensor_version)
add_subdirectory(permutation)
add_subdirectory(reduction)
