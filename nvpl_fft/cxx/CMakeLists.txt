# NVPL FFT: CXX Examples

## Sources
# CXX FFTW API examples
set(NVPL_FFTW_EXAMPLES
    1d_c2c_single
    1d_c2c_double
    c2c_c2r_r2c_single_apis
    include_header
)


# CXX FFTW API OMP examples
set(NVPL_FFTW_EXAMPLES_OMP
    c2c_single_many
    c2c_double_many
    r2c_c2r_single_many
    r2c_c2r_double_many
    c2c_single_withomp
    r2c_c2r_single_withomp
    auxiliary_apis
    c2c_c2r_r2c_many_bench
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
    foreach (THREAD 1 2 3 4 5 6 7 8 16 32 64 72 144)
        if (${THREAD} EQUAL 1)
            # 1D tests
            foreach(SIZE 1 2 3 10 12 34 123 1234 12345 123456)
                add_test(NAME "nvpl_fft.test.1d_c2c_${PREC}_example.${SIZE}" COMMAND 1d_c2c_${PREC}_example ${SIZE})
            endforeach()
        endif()
        # 1D tests
        foreach(SIZE 1 2 3 4 5 6 7 8 9 10 12 14 15 16 18 34 123 1234 12345 123456)
            add_test(NAME "nvpl_fft.test.c2c_${PREC}_many_example.${THREAD}.${SIZE}" COMMAND c2c_${PREC}_many_example ${THREAD} ${SIZE})
            add_test(NAME "nvpl_fft.test.r2c_c2r_${PREC}_many_example.${THREAD}.${SIZE}" COMMAND r2c_c2r_${PREC}_many_example  ${THREAD} ${SIZE})
        endforeach()
        # 2D tests
        foreach(SIZE 1 2 3 4 5 6 7 8 9 10 12 14 15 16 18 34 50 78 123 1234)
            add_test(NAME "nvpl_fft.test.c2c_${PREC}_many_example.${THREAD}.${SIZE}.${SIZE}" COMMAND c2c_${PREC}_many_example  ${THREAD} ${SIZE} ${SIZE})
            add_test(NAME "nvpl_fft.test.r2c_c2r_${PREC}_many_example.${THREAD}.${SIZE}.${SIZE}" COMMAND r2c_c2r_${PREC}_many_example  ${THREAD} ${SIZE} ${SIZE})
        endforeach()
        # 3D tests
        foreach(SIZE 1 2 3 4 5 6 7 8 9 10 12 14 15 16 18 34 50 78 128)
            add_test(NAME "nvpl_fft.test.c2c_${PREC}_many_example.${THREAD}.${SIZE}.${SIZE}.${SIZE}"     COMMAND c2c_${PREC}_many_example ${THREAD} ${SIZE} ${SIZE} ${SIZE})
            add_test(NAME "nvpl_fft.test.r2c_c2r_${PREC}_many_example.${THREAD}.${SIZE}.${SIZE}.${SIZE}" COMMAND r2c_c2r_${PREC}_many_example ${THREAD} ${SIZE} ${SIZE} ${SIZE})
        endforeach()
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

# API tests
add_test(NAME "nvpl_fft.test.auxiliary_apis_example"           COMMAND auxiliary_apis_example)
add_test(NAME "nvpl_fft.test.c2c_c2r_r2c_single_apis.10"       COMMAND c2c_c2r_r2c_single_apis_example 10)
add_test(NAME "nvpl_fft.test.c2c_c2r_r2c_single_apis.10.20"    COMMAND c2c_c2r_r2c_single_apis_example 10 20)
add_test(NAME "nvpl_fft.test.c2c_c2r_r2c_single_apis.10.20.30" COMMAND c2c_c2r_r2c_single_apis_example 10 20 30)

# Header Include test
add_test(NAME "nvpl_fft.test.include_header_example" COMMAND include_header_example)

# 1D/2D/3D Benchmark
add_test(NAME "nvpl_fft.test.c2c_c2r_r2c_many_bench_example.fp32.c2c.p_2357" COMMAND c2c_c2r_r2c_many_bench_example --prec fp32 --fft_type c2c --mode oop --cat p_2357)
add_test(NAME "nvpl_fft.test.c2c_c2r_r2c_many_bench_example.fp64.c2c.p_2357" COMMAND c2c_c2r_r2c_many_bench_example --prec fp64 --fft_type c2c --mode oop --cat p_2357)
