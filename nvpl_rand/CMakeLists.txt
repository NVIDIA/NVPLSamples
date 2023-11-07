# ******************************************
# NVPL RAND Examples
# ******************************************
set(NVPL_RAND_EXAMPLES_STANDALONE TRUE CACHE BOOL "")
set(NVPL_RAND_EXAMPLES_PROJECT_IS_TOP_LEVEL TRUE CACHE BOOL "")

if(NVPL_RAND_EXAMPLES_STANDALONE)
    cmake_minimum_required(VERSION 3.12)
    project(nvpl_rand_examples LANGUAGES CXX)
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified. Set CMAKE_BUILD_TYPE variable to change build type.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(NVPL_RAND_EXAMPLES_PROJECT_IS_TOP_LEVEL OR EXAMPLE_INSTALLED_VERSION)
    enable_testing()
    find_package(nvpl REQUIRED COMPONENTS rand)

    if(NOT TARGET nvpl::rand)
        message(FATAL_ERROR "Required config package not found!")
        return()
    endif()

    include(GNUInstallDirs)
    set(NVPL_RAND_EXAMPLES_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})
endif()

if(NVPL_RAND_EXAMPLES_PROJECT_IS_TOP_LEVEL)
    # Global CXX flags/options
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# CudaToolkit, used in test_mt_correctness_curand_legacy
# find_package(CUDAToolkit)
set(CUDAToolkit_FOUND FALSE CACHE BOOL "")

if(CUDAToolkit_FOUND)
    message(STATUS "CUDAToolkit ${CUDAToolkit_VERSION} found: example_mt_generate_bits_compare_curandxx uses CURAND")
else()
    message(STATUS "CUDAToolkit NOT FOUND: example_mt_generate_bits_compare_curandxx not use CURAND")
endif()

# ###############################################################
# add_rand_example
# ###############################################################
function(add_rand_example2 GROUP_TARGET RAND_MT EXAMPLE_NAME EXAMPLE_TARGET EXAMPLE_SOURCES)
    add_executable(${EXAMPLE_TARGET} ${EXAMPLE_SOURCES})

    if(${RAND_MT})
        # Link multi-thread NVPL RAND (includes single-thread API too)
        target_link_libraries(${EXAMPLE_TARGET} PRIVATE nvpl::rand_mt)
    else()
        # Link single-thread NVPL RAND
        target_link_libraries(${EXAMPLE_TARGET} PRIVATE nvpl::rand)
    endif()

    if(NOT NVPL_RAND_EXAMPLES_PROJECT_IS_TOP_LEVEL)
        target_link_libraries(${EXAMPLE_TARGET}
            PRIVATE
            nvpl_rand_cxx_warnings_flags
            nvpl_rand_cxx_arch_flags
        )
    endif()

    set_target_properties(${EXAMPLE_TARGET}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/example/rand"
    )
    add_test(NAME ${EXAMPLE_NAME} COMMAND ${EXAMPLE_TARGET})
    set_tests_properties(${EXAMPLE_NAME}
        PROPERTIES
        LABELS "NVPL_RAND"
    )
    add_dependencies(${GROUP_TARGET} ${EXAMPLE_TARGET})

    if(NOT NVPL_RAND_EXAMPLES_PROJECT_IS_TOP_LEVEL)
        file(RELATIVE_PATH _rpath /${NVPL_RAND_EXAMPLES_INSTALL_DIR} /${CMAKE_INSTALL_LIBDIR})
        set_target_properties(${EXAMPLE_TARGET} PROPERTIES INSTALL_RPATH \$ORIGIN/${_rpath})
    endif()

    install(
        TARGETS ${EXAMPLE_TARGET}
        RUNTIME
        DESTINATION ${NVPL_RAND_EXAMPLES_INSTALL_DIR}
        COMPONENT Tests
    )
endfunction()

function(add_rand_example GROUP_TARGET EXAMPLE_NAME EXAMPLE_SOURCES)
    list(GET EXAMPLE_SOURCES 0 EXAMPLE_MAIN_SOURCE)
    get_filename_component(EXAMPLE_TARGET ${EXAMPLE_MAIN_SOURCE} NAME_WE)
    add_rand_example2("${GROUP_TARGET}" FALSE "${EXAMPLE_NAME}" "${EXAMPLE_TARGET}" "${EXAMPLE_SOURCES}")
endfunction()

function(add_rand_mt_example GROUP_TARGET EXAMPLE_NAME EXAMPLE_SOURCES)
    list(GET EXAMPLE_SOURCES 0 EXAMPLE_MAIN_SOURCE)
    get_filename_component(EXAMPLE_TARGET ${EXAMPLE_MAIN_SOURCE} NAME_WE)
    add_rand_example2("${GROUP_TARGET}" TRUE "${EXAMPLE_NAME}" "${EXAMPLE_TARGET}" "${EXAMPLE_SOURCES}")
endfunction()

# ###############################################################
# add_rand_mt_custom_example_curand
# ###############################################################
function(add_rand_mt_example_curand GROUP_TARGET EXAMPLE_NAME EXAMPLE_TARGET EXAMPLE_SOURCES)
    add_executable(${EXAMPLE_TARGET} ${EXAMPLE_SOURCES})

    # Link multi-thread NVPL RAND (includes single-thread API too)
    target_link_libraries(${EXAMPLE_TARGET}
        PRIVATE
        nvpl::rand_mt)
        # $<$<BOOL:${CUDAToolkit_FOUND}>:CUDA::curand CUDA::cudart>)

    if(NOT NVPL_RAND_EXAMPLES_PROJECT_IS_TOP_LEVEL)
        target_link_libraries(${EXAMPLE_TARGET}
            PRIVATE
            nvpl_rand_cxx_warnings_flags
            nvpl_rand_cxx_arch_flags
        )
    endif()

    if(CUDAToolkit_FOUND)
        target_compile_definitions(${EXAMPLE_TARGET} PRIVATE NVPL_RAND_EXAMPLE_CURAND_AVAILABLE)
    endif()

    set_target_properties(${EXAMPLE_TARGET}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/example/rand"
    )
    add_test(NAME ${EXAMPLE_NAME} COMMAND ${EXAMPLE_TARGET})
    set_tests_properties(${EXAMPLE_NAME}
        PROPERTIES
        LABELS "NVPL_RAND"
    )
    add_dependencies(${GROUP_TARGET} ${EXAMPLE_TARGET})

    if(NOT NVPL_RAND_EXAMPLES_PROJECT_IS_TOP_LEVEL)
        file(RELATIVE_PATH _rpath /${NVPL_RAND_EXAMPLES_INSTALL_DIR} /${CMAKE_INSTALL_LIBDIR})
        set_target_properties(${EXAMPLE_TARGET} PROPERTIES INSTALL_RPATH \$ORIGIN/${_rpath})
    endif()

    install(
        TARGETS ${EXAMPLE_TARGET}
        RUNTIME
        DESTINATION ${NVPL_RAND_EXAMPLES_INSTALL_DIR}
        COMPONENT Tests
    )
endfunction()

function(add_rand_mt_custom_example_curand GROUP_TARGET EXAMPLE_NAME EXAMPLE_SOURCES)
    list(GET EXAMPLE_SOURCES 0 EXAMPLE_MAIN_SOURCE)
    get_filename_component(EXAMPLE_TARGET ${EXAMPLE_MAIN_SOURCE} NAME_WE)
    add_rand_mt_example_curand("${GROUP_TARGET}" "${EXAMPLE_NAME}" "${EXAMPLE_TARGET}" "${EXAMPLE_SOURCES}")
endfunction()

# ###############################################################
# NVPL_RAND Examples
# ###############################################################
add_custom_target(nvpl_rand_examples)
add_rand_example(nvpl_rand_examples "nvplRAND.example.verify_version" verify_version.cpp)
add_rand_example(nvpl_rand_examples "nvplRAND.example.rand_st_distribution_config" rand_st_distribution_config.cpp)
add_rand_mt_example(nvpl_rand_examples "nvplRAND.example.rand_generate_normal_fp64" rand_mt_generate_normal_fp64.cpp)
add_rand_mt_example(nvpl_rand_examples "nvplRANDMT.example.rand_mt_generate_bits_compare_threads" rand_mt_generate_bits_compare_threads.cpp)
add_rand_mt_example(nvpl_rand_examples "nvplRANDMT.example.rand_mt_generate_bits_compare_st" rand_mt_generate_bits_compare_st.cpp)

add_rand_mt_custom_example_curand(nvpl_rand_examples "nvplRAND.example.rand_mt_generate_bits_compare_curand" rand_mt_generate_bits_compare_curand.cpp)
add_rand_mt_custom_example_curand(nvpl_rand_examples "nvplRAND.example.rand_mt_generate_bits_compare_curand_sobol" rand_mt_generate_bits_compare_curand_sobol.cpp)
