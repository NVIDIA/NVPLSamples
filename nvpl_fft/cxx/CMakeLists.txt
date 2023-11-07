# NVPL FFT: CXX Examples

## Sources
# CXX FFTW API examples
set(NVPL_FFTW_EXAMPLES
    1d_c2c_single
    1d_c2c_double
    c2c_single_many
    c2c_double_many
    r2c_c2r_single_many
    r2c_c2r_double_many
    c2c_c2r_r2c_single_apis
    auxiliary_apis
)

# CXX FFTW API OMP examples
set(NVPL_FFTW_EXAMPLES_OMP
    c2c_single_withomp
    r2c_c2r_single_withomp
)

## Target executables
foreach(name IN LISTS NVPL_FFTW_EXAMPLES NVPL_FFTW_EXAMPLES_OMP)
    add_executable(${name}_example ${name}_example.cpp)
    target_link_libraries(${name}_example PRIVATE nvpl::fftw)
    if(name IN_LIST NVPL_FFTW_EXAMPLES_OMP)
        target_link_libraries(${name}_example PRIVATE OpenMP::OpenMP_CXX)
    endif()
    target_compile_options(${name}_example PRIVATE -Wall -Wextra)
    if(EXAMPLE_INSTALL_DIR)
        install(TARGETS ${name}_example RUNTIME DESTINATION ${EXAMPLE_INSTALL_DIR}/cxx COMPONENT Tests)
    endif()
endforeach()

## CTests
foreach(PREC single double)
    # 1D tests
    foreach(SIZE 1 2 3 10 12 34 123 1234 12345 123456)
        add_test(NAME "nvpl_fft.test.1d_c2c_${PREC}_example.${SIZE}"       COMMAND 1d_c2c_${PREC}_example ${SIZE})
        add_test(NAME "nvpl_fft.test.c2c_${PREC}_many_example.${SIZE}"     COMMAND c2c_${PREC}_many_example ${SIZE})
        add_test(NAME "nvpl_fft.test.r2c_c2r_${PREC}_many_example.${SIZE}" COMMAND r2c_c2r_${PREC}_many_example ${SIZE})
    endforeach()

    # 2D tests
    foreach(SIZE 1 2 3 10 12 34 50 78 123 1234)
        add_test(NAME "nvpl_fft.test.c2c_${PREC}_many_example.${SIZE}.${SIZE}"     COMMAND c2c_${PREC}_many_example ${SIZE} ${SIZE})
        add_test(NAME "nvpl_fft.test.r2c_c2r_${PREC}_many_example.${SIZE}.${SIZE}" COMMAND r2c_c2r_${PREC}_many_example ${SIZE} ${SIZE})
    endforeach()

    # 3D tests
    foreach(SIZE 1 2 3 10 12 34 50 78 128)
        add_test(NAME "nvpl_fft.test.c2c_${PREC}_many_example.${SIZE}.${SIZE}.${SIZE}"     COMMAND c2c_${PREC}_many_example ${SIZE} ${SIZE} ${SIZE})
        add_test(NAME "nvpl_fft.test.r2c_c2r_${PREC}_many_example.${SIZE}.${SIZE}.${SIZE}" COMMAND r2c_c2r_${PREC}_many_example ${SIZE} ${SIZE} ${SIZE})
    endforeach()
endforeach()

# nvplfft + omp
# 1D tests
foreach(SIZE 1 2 3 10 12 34 123 1234 12345 123456)
    add_test(NAME "nvpl_fft.test.c2c_single_withomp_example.${SIZE}"     COMMAND c2c_single_withomp_example ${SIZE})
    add_test(NAME "nvpl_fft.test.r2c_c2r_single_withomp_example.${SIZE}" COMMAND r2c_c2r_single_withomp_example ${SIZE})
endforeach()

# 2D tests
foreach(SIZE 1 2 3 10 12 34 50 78 123)
    add_test(NAME "nvpl_fft.test.c2c_single_withomp_example.${SIZE}.${SIZE}"     COMMAND c2c_single_withomp_example ${SIZE} ${SIZE})
    add_test(NAME "nvpl_fft.test.r2c_c2r_single_withomp_example.${SIZE}.${SIZE}" COMMAND r2c_c2r_single_withomp_example ${SIZE} ${SIZE})
endforeach()

# 3D tests
foreach(SIZE 1 2 3 10 12 35 128)
    add_test(NAME "nvpl_fft.test.c2c_single_withomp_example.${SIZE}.${SIZE}.${SIZE}"     COMMAND c2c_single_withomp_example ${SIZE} ${SIZE} ${SIZE})
    add_test(NAME "nvpl_fft.test.r2c_c2r_single_withomp_example.${SIZE}.${SIZE}.${SIZE}" COMMAND r2c_c2r_single_withomp_example ${SIZE} ${SIZE} ${SIZE})
endforeach()

# API test
add_test(NAME "nvpl_fft.test.auxiliary_apis_example" COMMAND auxiliary_apis_example)

