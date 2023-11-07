file(RELATIVE_PATH this_dir ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.f)
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/common_func.f)
foreach(src ${SOURCES})
    get_filename_component(name "${src}" NAME_WE)
    set(name f77_${INT_TYPE}_${THR_TYPE}_${name})
    add_executable(${name} ${src} common_func.f)

    # ILP64 interface assumes Fortran sources are built with 8-byte integers
    if (INT_TYPE STREQUAL ilp64)
        target_compile_options(${name} PRIVATE
            $<$<Fortran_COMPILER_ID:GNU>:-fdefault-integer-8>
            $<$<Fortran_COMPILER_ID:NVHPC>:-i8 -Mlarge_arrays>
        )
    endif()

    #Link NVPL BLAS variant with specified INT and THR types
    target_link_libraries(${name} PRIVATE nvpl::blas_${INT_TYPE}_${THR_TYPE})

    if(THR_TYPE STREQUAL omp)
        # Enable OpenMP in example source
        target_link_libraries(${name} PRIVATE OpenMP::OpenMP_Fortran)
    endif()

    add_test(${name} ${name})
    install(TARGETS ${name}
            RUNTIME DESTINATION ${examples_install_prefix}/${this_dir}/${INT_TYPE}_${THR_TYPE}_dynamic
            COMPONENT Tests)
endforeach()
