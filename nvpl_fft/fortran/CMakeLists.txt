# NVPL FFT: Fortran Examples

## Sources
# Fotran FFTW API examples
set(NVPL_FFTW_FORTRAN_03_EXAMPLES
    c2c_c2r_r2c_double_apis
    c2c_c2r_r2c_single_apis
)

set(NVPL_FFTW_FORTRAN_77_EXAMPLES
    c2c_c2r_r2c_double_apis
)

## Target executables
foreach(name IN LISTS NVPL_FFTW_FORTRAN_03_EXAMPLES)
    # .f90 is used as the file extension for all f03 examples as older versions of cmake do not recognize .f03 as a valid fortran file extension
    # (https://gitlab.kitware.com/cmake/cmake/-/merge_requests/8330). As mentioned here: https://fortranwiki.org/fortran/show/File+extensions
    # it is common to use .f90 to denote any fortran file containing "modern"/free-form fortran.
    add_executable(${name}_example_03 ${name}_example.f90)
    set_property(TARGET ${name}_example_03 PROPERTY LINKER_LANGUAGE Fortran)
    target_link_libraries(${name}_example_03 PRIVATE nvpl::fftw)
    # -fall-intrinsics is for EXIT()
    target_compile_options(${name}_example_03 PRIVATE -Wall -Wextra -Wno-unused-parameter -std=f2003 -fall-intrinsics)
    if(EXAMPLE_INSTALL_DIR)
        install(TARGETS ${name}_example_03 RUNTIME DESTINATION ${EXAMPLE_INSTALL_DIR}/fortran COMPONENT Tests)
    endif()

    add_test(NAME "nvpl_fft.test.f03.${name}" COMMAND ${name}_example_03)
endforeach()

foreach(name IN LISTS NVPL_FFTW_FORTRAN_77_EXAMPLES)
    add_executable(${name}_example_77 ${name}_example.f)
    set_property(TARGET ${name}_example_77 PROPERTY LINKER_LANGUAGE Fortran)
    target_link_libraries(${name}_example_77 PRIVATE nvpl::fftw)
    # -fall-intrinsics is for EXIT()
    target_compile_options(${name}_example_77 PRIVATE -Wall -Wextra -Wno-unused-parameter -std=legacy -fall-intrinsics)
    if(EXAMPLE_INSTALL_DIR)
        install(TARGETS ${name}_example_77 RUNTIME DESTINATION ${EXAMPLE_INSTALL_DIR}/fortran COMPONENT Tests)
    endif()

    add_test(NAME "nvpl_fft.test.f77.${name}" COMMAND ${name}_example_77)
endforeach()
