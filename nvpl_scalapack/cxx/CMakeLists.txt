# NVPL SCALAPACK: CXX Examples
set(NVPL_SCALAPACK_EXAMPLES_CXX
    nvpl_scalapack_example_pdgemm
    nvpl_scalapack_example_pdgesv    
)

foreach(name IN LISTS NVPL_SCALAPACK_EXAMPLES_CXX)
    # Create test executable
    set(exe ${name}_${INT_TYPE}_${MPI_INTERFACE}_${THR_TYPE})
    add_executable(${exe} ${name}.cpp)
    target_compile_options(${exe} PRIVATE -Wall -Wextra)
    # Link to NVPL Libraries
    target_link_libraries(${exe}
        PUBLIC
            nvpl::scalapack_${INT_TYPE}
            nvpl::lapack_${INT_TYPE}_${THR_TYPE}
            nvpl::blacs_${INT_TYPE}_${MPI_INTERFACE}
            MPI::MPI_C
    )
    if(MPI_INTERFACE MATCHES openmpi)
        # Don't use OpenMPI's CXX interface
        target_compile_definitions(${exe} PRIVATE OMPI_SKIP_MPICXX)
    endif()

    # Add CTest
    set(test_name NVPL::Example::CXX::${exe})
    add_test(NAME ${test_name}
        COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${MPI_NUMPROC} ${CMAKE_CURRENT_BINARY_DIR}/${exe}
    )
    set_tests_properties(${test_name} PROPERTIES ${TEST_PROPERTIES})
    install(TARGETS ${exe} RUNTIME DESTINATION ${EXAMPLE_INSTALL_DIR} COMPONENT Tests)
endforeach()
