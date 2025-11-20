# NVPL LAPACK: C++ Examples
cmake_minimum_required(VERSION 3.12)

# CXX build macro (for direct C++ examples)
macro(addCXXExample name)
  add_executable(ex_cpp_${name}_ilp64_seq ex_${name}.cpp utils.cpp)
  target_link_libraries(ex_cpp_${name}_ilp64_seq PUBLIC nvpl::lapack_ilp64_seq)

  add_executable(ex_cpp_${name}_lp64_seq ex_${name}.cpp utils.cpp)
  target_link_libraries(ex_cpp_${name}_lp64_seq PUBLIC nvpl::lapack_lp64_seq)

  add_executable(ex_cpp_${name}_ilp64_omp ex_${name}.cpp utils.cpp)
  target_link_libraries(ex_cpp_${name}_ilp64_omp PUBLIC nvpl::lapack_ilp64_omp)
  target_link_libraries(ex_cpp_${name}_ilp64_omp PRIVATE OpenMP::OpenMP_CXX)

  add_executable(ex_cpp_${name}_lp64_omp ex_${name}.cpp utils.cpp)
  target_link_libraries(ex_cpp_${name}_lp64_omp PUBLIC nvpl::lapack_lp64_omp)
  target_link_libraries(ex_cpp_${name}_lp64_omp PRIVATE OpenMP::OpenMP_CXX)

  set(CXX_EXES ${CXX_EXES} ex_cpp_${name}_ilp64_seq ex_cpp_${name}_lp64_seq
               ex_cpp_${name}_ilp64_omp ex_cpp_${name}_lp64_omp)
endmacro(addCXXExample)

# OpenMP-only CXX build macro (for direct C++ examples)
macro(addCXXOMPExample name)
  add_executable(ex_cpp_${name}_ilp64_omp ex_${name}.cpp)
  target_link_libraries(ex_cpp_${name}_ilp64_omp PUBLIC nvpl::lapack_ilp64_omp)
  target_link_libraries(ex_cpp_${name}_ilp64_omp PRIVATE OpenMP::OpenMP_CXX)

  add_executable(ex_cpp_${name}_lp64_omp ex_${name}.cpp)
  target_link_libraries(ex_cpp_${name}_lp64_omp PUBLIC nvpl::lapack_lp64_omp)
  target_link_libraries(ex_cpp_${name}_lp64_omp PRIVATE OpenMP::OpenMP_CXX)

  set(CXX_EXES ${CXX_EXES} ex_cpp_${name}_ilp64_omp ex_cpp_${name}_lp64_omp)
endmacro(addCXXOMPExample)

# C++ Examples
addCXXExample(dgels)
addCXXExample(sgels)
addCXXExample(cgels)
addCXXExample(zgels)
addCXXExample(dgesv)
addCXXExample(sgesv)
addCXXExample(cgesv)
addCXXExample(zgesv)
addCXXExample(dposv)
addCXXExample(sposv)
addCXXExample(cposv)
addCXXExample(zposv)
addCXXExample(dsyev)
addCXXExample(ssyev)
addCXXExample(cheev)
addCXXExample(zheev)

# OpenMP-only C++ Examples
addCXXOMPExample(thread_control)
addCXXOMPExample(thread_control_for_library_developers)
addCXXOMPExample(batched_getrf)

# LAPACKE Examples (C interface to LAPACK)
addCXXExample(lapacke_dgels_colmajor)
addCXXExample(lapacke_sgels_colmajor)
addCXXExample(lapacke_cgels_colmajor)
addCXXExample(lapacke_zgels_colmajor)
addCXXExample(lapacke_dgels_rowmajor)
addCXXExample(lapacke_sgels_rowmajor)
addCXXExample(lapacke_cgels_rowmajor)
addCXXExample(lapacke_zgels_rowmajor)
addCXXExample(lapacke_dgesv_colmajor)
addCXXExample(lapacke_sgesv_colmajor)
addCXXExample(lapacke_cgesv_colmajor)
addCXXExample(lapacke_zgesv_colmajor)
addCXXExample(lapacke_dgesv_rowmajor)
addCXXExample(lapacke_sgesv_rowmajor)
addCXXExample(lapacke_cgesv_rowmajor)
addCXXExample(lapacke_zgesv_rowmajor)
addCXXExample(lapacke_dposv_colmajor)
addCXXExample(lapacke_sposv_colmajor)
addCXXExample(lapacke_cposv_colmajor)
addCXXExample(lapacke_zposv_colmajor)
addCXXExample(lapacke_dposv_rowmajor)
addCXXExample(lapacke_sposv_rowmajor)
addCXXExample(lapacke_cposv_rowmajor)
addCXXExample(lapacke_zposv_rowmajor)
addCXXExample(lapacke_dsyev_colmajor)
addCXXExample(lapacke_ssyev_colmajor)
addCXXExample(lapacke_cheev_colmajor)
addCXXExample(lapacke_zheev_colmajor)
addCXXExample(lapacke_dsyev_rowmajor)
addCXXExample(lapacke_ssyev_rowmajor)
addCXXExample(lapacke_cheev_rowmajor)
addCXXExample(lapacke_zheev_rowmajor)

# CTest
enable_testing()
foreach(exe IN LISTS CXX_EXES)
  add_test(${exe} ${exe})
endforeach()

include(GNUInstallDirs)
install(TARGETS ${CXX_EXES}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

