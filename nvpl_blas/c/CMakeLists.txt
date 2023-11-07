file(RELATIVE_PATH this_dir ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
foreach(src ${SOURCES})
    get_filename_component(name "${src}" NAME_WE)
    set(name c_${INT_TYPE}_${THR_TYPE}_${name})
    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE m)

    #Link NVPL BLAS variant with specified INT and THR types
    target_link_libraries(${name} PRIVATE nvpl::blas_${INT_TYPE}_${THR_TYPE})

    if(THR_TYPE STREQUAL omp)
        # Enable OpenMP in example source
        target_link_libraries(${name} PRIVATE OpenMP::OpenMP_C)
    endif()

    add_test(${name} ${name})
    install(TARGETS ${name}
            RUNTIME DESTINATION ${examples_install_prefix}/${this_dir}/${INT_TYPE}_${THR_TYPE}_dynamic
            COMPONENT Tests)
endforeach()
