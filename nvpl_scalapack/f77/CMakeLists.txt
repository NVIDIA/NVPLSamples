# NVPL SCALAPACK: Fortran Examples
set(EXAMPLE_DATA
    SCAEX.dat
    CSCAEXMAT.dat
    CSCAEXRHS.dat
    DSCAEXMAT.dat
    DSCAEXRHS.dat
    SSCAEXMAT.dat
    SSCAEXRHS.dat
    ZSCAEXMAT.dat
    ZSCAEXRHS.dat
)
# Install data files to binary dir
file(COPY ${EXAMPLE_DATA} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(EXAMPLE_COMMON_SOURCES
    pdscaexinfo.f
)

set(EXAMPLE_EXES
    pcscaex
    pdscaex
    psscaex
    pzscaex
)

foreach(name IN LISTS EXAMPLE_EXES)
    # Create test executable
    set(exe ${name}_${INT_TYPE}_${MPI_INTERFACE}_${THR_TYPE})
    add_executable(${exe} ${name}.f ${EXAMPLE_COMMON_SOURCES})

    #Set warnings
    target_compile_options(${exe} PRIVATE -Wall -Wextra)
    if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
        target_compile_options(${exe} PRIVATE -Wno-unused-parameter)
        target_link_libraries(${exe} PRIVATE -Wno-lto-type-mismatch)
    endif()

    # Link to NVPL Libraries
    target_link_libraries(${exe}
        PRIVATE
            nvpl::scalapack_${INT_TYPE}
            nvpl::lapack_${INT_TYPE}_${THR_TYPE}
            nvpl::blacs_${INT_TYPE}_${MPI_INTERFACE}
            MPI::MPI_Fortran
    )
    # Fortran compile options
    target_compile_options(${exe} PRIVATE -cpp)
    if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
        target_compile_options(${exe} PRIVATE -frecursive)
    endif()

    # Configure Fortran Integer ABI
    if(INT_TYPE STREQUAL "ilp64")
        target_compile_definitions(${exe} PRIVATE NVPL_ILP64)
        if(CMAKE_Fortran_COMPILER_ID STREQUAL GNU)
            target_compile_options(${exe} PRIVATE -fdefault-integer-8)
        elseif(CMAKE_Fortran_COMPILER_ID STREQUAL NVHPC)
            target_compile_options(${exe} PRIVATE -i8)
        endif()
    endif()

    # Add CTest
    set(test_name NVPL::Example::Fortran::${exe})
    add_test(NAME ${test_name}
        COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${MPI_NUMPROC} ${CMAKE_CURRENT_BINARY_DIR}/${exe}
    )
    set_tests_properties(${test_name} PROPERTIES ${TEST_PROPERTIES})
    install(TARGETS ${exe} RUNTIME DESTINATION ${EXAMPLE_INSTALL_DIR} COMPONENT Tests)
endforeach()
