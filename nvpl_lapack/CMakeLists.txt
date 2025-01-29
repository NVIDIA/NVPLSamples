# NVPL LAPACK: Examples
#
cmake_minimum_required(VERSION 3.12)
project(
  nvpl_lapack_examples
  LANGUAGES Fortran C CXX
)

# Find NVPL LAPACK
find_package(OpenMP REQUIRED COMPONENTS CXX)
find_package(nvpl REQUIRED COMPONENTS lapack)

# Fortran ILP64 Flags NOTE: CMake generator-expression
# $<CMAKE_Fortran_COMPILER_ID:> requires CMake-3.14
if(CMAKE_Fortran_COMPILER_ID MATCHES GNU)
  set(ILP64_FORTRAN_COMPILE_FLAG -fdefault-integer-8)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES NVHPC)
  set(ILP64_FORTRAN_COMPILE_FLAG -i8)
else()
  message(
    SEND_ERROR
      "Unsupported CMAKE_Fortran_COMPILER_ID=${CMAKE_Fortran_COMPILER_ID}"
  )
endif()

# OpenMP flags
set(OPENMP_COMPILE_FLAG -fopenmp)

# Fortran build macro
macro(addFortranExample name)
  add_executable(ex_${name}_ilp64_seq ex_${name}.f90)
  target_link_libraries(ex_${name}_ilp64_seq PRIVATE nvpl::lapack_ilp64_seq)
  target_compile_options(
    ex_${name}_ilp64_seq PRIVATE ${ILP64_FORTRAN_COMPILE_FLAG}
  )

  add_executable(ex_${name}_lp64_seq ex_${name}.f90)
  target_link_libraries(ex_${name}_lp64_seq PRIVATE nvpl::lapack_lp64_seq)

  add_executable(ex_${name}_ilp64_omp ex_${name}.f90)
  target_link_libraries(ex_${name}_ilp64_omp PRIVATE nvpl::lapack_ilp64_omp)
  target_compile_options(
    ex_${name}_ilp64_omp PRIVATE ${ILP64_FORTRAN_COMPILE_FLAG}
  )

  add_executable(ex_${name}_lp64_omp ex_${name}.f90)
  target_link_libraries(ex_${name}_lp64_omp PRIVATE nvpl::lapack_lp64_omp)

  set(FORTRAN_EXES ${FORTRAN_EXES} ex_${name}_ilp64_seq ex_${name}_lp64_seq
                   ex_${name}_ilp64_omp ex_${name}_lp64_omp
  )
endmacro(addFortranExample)

# CXX build macro
macro(addCXXExample name)
  add_executable(ex_${name}_ilp64_seq ex_${name}.cpp utils.cpp)
  target_link_libraries(ex_${name}_ilp64_seq PUBLIC nvpl::lapack_ilp64_seq)

  add_executable(ex_${name}_lp64_seq ex_${name}.cpp utils.cpp)
  target_link_libraries(ex_${name}_lp64_seq PUBLIC nvpl::lapack_lp64_seq)

  add_executable(ex_${name}_ilp64_omp ex_${name}.cpp utils.cpp)
  target_link_libraries(ex_${name}_ilp64_omp PUBLIC nvpl::lapack_ilp64_omp)
  # Enable OpenMP in example source
  target_compile_options(ex_${name}_ilp64_omp PRIVATE ${OPENMP_COMPILE_FLAG})
  target_link_libraries(ex_${name}_ilp64_omp PRIVATE OpenMP::OpenMP_CXX)
  target_link_libraries(ex_${name}_ilp64_omp PRIVATE ${OPENMP_COMPILE_FLAG})

  add_executable(ex_${name}_lp64_omp ex_${name}.cpp utils.cpp)
  target_link_libraries(ex_${name}_lp64_omp PUBLIC nvpl::lapack_lp64_omp)
  # Enable OpenMP in example source
  target_compile_options(ex_${name}_lp64_omp PRIVATE ${OPENMP_COMPILE_FLAG})
  target_link_libraries(ex_${name}_lp64_omp PRIVATE OpenMP::OpenMP_CXX)
  target_link_libraries(ex_${name}_lp64_omp PRIVATE ${OPENMP_COMPILE_FLAG})

  set(CXX_EXES ${CXX_EXES} ex_${name}_ilp64_seq ex_${name}_lp64_seq
               ex_${name}_ilp64_omp ex_${name}_lp64_omp
  )
endmacro(addCXXExample)

# OpenMP-only CXX build macro
macro(addCXXOMPExample name)

  add_executable(ex_${name}_ilp64_omp ex_${name}.cpp)
  target_link_libraries(ex_${name}_ilp64_omp PUBLIC nvpl::lapack_ilp64_omp)
  # Enable OpenMP in example source
  target_compile_options(ex_${name}_ilp64_omp PRIVATE ${OPENMP_COMPILE_FLAG})
  target_link_libraries(ex_${name}_ilp64_omp PRIVATE OpenMP::OpenMP_CXX)
  target_link_libraries(ex_${name}_ilp64_omp PRIVATE ${OPENMP_COMPILE_FLAG})

  add_executable(ex_${name}_lp64_omp ex_${name}.cpp)
  target_link_libraries(ex_${name}_lp64_omp PUBLIC nvpl::lapack_lp64_omp)
  # Enable OpenMP in example source
  target_compile_options(ex_${name}_lp64_omp PRIVATE ${OPENMP_COMPILE_FLAG})
  target_link_libraries(ex_${name}_lp64_omp PRIVATE OpenMP::OpenMP_CXX)
  target_link_libraries(ex_${name}_lp64_omp PRIVATE ${OPENMP_COMPILE_FLAG})

  set(CXX_EXES ${CXX_EXES} ex_${name}_ilp64_omp ex_${name}_lp64_omp)
endmacro(addCXXOMPExample)

# NVPL LAPACK: Fortran Examples
addfortranexample(getrf_nopivot)
addfortranexample(potrf)

# NVPL LAPACK: CXX Examples
addcxxexample(dgels)
addcxxexample(sgels)
addcxxexample(cgels)
addcxxexample(zgels)
addcxxexample(lapacke_dgels_colmajor)
addcxxexample(lapacke_sgels_colmajor)
addcxxexample(lapacke_cgels_colmajor)
addcxxexample(lapacke_zgels_colmajor)
addcxxexample(lapacke_dgels_rowmajor)
addcxxexample(lapacke_sgels_rowmajor)
addcxxexample(lapacke_cgels_rowmajor)
addcxxexample(lapacke_zgels_rowmajor)
addcxxexample(lapacke_dgesv_colmajor)
addcxxexample(lapacke_sgesv_colmajor)
addcxxexample(lapacke_cgesv_colmajor)
addcxxexample(lapacke_zgesv_colmajor)
addcxxexample(lapacke_dgesv_rowmajor)
addcxxexample(lapacke_sgesv_rowmajor)
addcxxexample(lapacke_cgesv_rowmajor)
addcxxexample(lapacke_zgesv_rowmajor)
addcxxexample(lapacke_dposv_colmajor)
addcxxexample(lapacke_sposv_colmajor)
addcxxexample(lapacke_cposv_colmajor)
addcxxexample(lapacke_zposv_colmajor)
addcxxexample(lapacke_dposv_rowmajor)
addcxxexample(lapacke_sposv_rowmajor)
addcxxexample(lapacke_cposv_rowmajor)
addcxxexample(lapacke_zposv_rowmajor)
addcxxexample(lapacke_dsyev_colmajor)
addcxxexample(lapacke_ssyev_colmajor)
addcxxexample(lapacke_cheev_colmajor)
addcxxexample(lapacke_zheev_colmajor)
addcxxexample(lapacke_dsyev_rowmajor)
addcxxexample(lapacke_ssyev_rowmajor)
addcxxexample(lapacke_cheev_rowmajor)
addcxxexample(lapacke_zheev_rowmajor)
addcxxompexample(thread_control)
addcxxompexample(thread_control_for_library_developers)

add_executable(ex_batched_getrf_ilp64_omp ex_batched_getrf.cpp)
target_link_libraries(
  ex_batched_getrf_ilp64_omp PRIVATE nvpl::lapack_ilp64_omp OpenMP::OpenMP_CXX
)
set(CXX_EXES ${CXX_EXES} ex_batched_getrf_ilp64_omp)

# CTest
enable_testing()
foreach(exe IN LISTS CXX_EXES FORTRAN_EXES)
  add_test(${exe} ${exe})
endforeach()

include(GNUInstallDirs)
install(TARGETS ${CXX_EXES} ${FORTRAN_EXES}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
