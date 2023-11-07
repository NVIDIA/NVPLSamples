file(RELATIVE_PATH this_dir ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
foreach(src ${SOURCES})
    get_filename_component(name "${src}" NAME_WE)
    set(name c_${name})
    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE nvpl_sparse)
    target_link_libraries(${name} PRIVATE OpenMP::OpenMP_C)
    add_test(${name} ${name})
    install(TARGETS ${name}
            RUNTIME DESTINATION ${examples_install_prefix}/${this_dir}
            COMPONENT Tests)
endforeach()
