# NVPL LAPACK: Fortran 77 Examples
cmake_minimum_required(VERSION 3.12)

# Fortran ILP64 Flags NOTE: CMake generator-expression
# $<CMAKE_Fortran_COMPILER_ID:> requires CMake-3.14
if(CMAKE_Fortran_COMPILER_ID MATCHES GNU)
  set(ILP64_FORTRAN_COMPILE_FLAG -fdefault-integer-8)
  set(FORTRAN_FREE_FORM_FLAG -ffree-form)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES NVHPC)
  set(ILP64_FORTRAN_COMPILE_FLAG -i8)
  set(FORTRAN_FREE_FORM_FLAG -Mfree)
else()
  message(
    SEND_ERROR
      "Unsupported CMAKE_Fortran_COMPILER_ID=${CMAKE_Fortran_COMPILER_ID}"
  )
endif()

# Fortran build macro
macro(addFortranExample name)
  add_executable(ex_f77_${name}_ilp64_seq ex_${name}.f common_func.f)
  target_link_libraries(ex_f77_${name}_ilp64_seq PRIVATE nvpl::lapack_ilp64_seq)
  target_compile_options(
    ex_f77_${name}_ilp64_seq PRIVATE ${ILP64_FORTRAN_COMPILE_FLAG} ${FORTRAN_FREE_FORM_FLAG}
  )

  add_executable(ex_f77_${name}_lp64_seq ex_${name}.f common_func.f)
  target_link_libraries(ex_f77_${name}_lp64_seq PRIVATE nvpl::lapack_lp64_seq)
  target_compile_options(
    ex_f77_${name}_lp64_seq PRIVATE ${FORTRAN_FREE_FORM_FLAG}
  )

  add_executable(ex_f77_${name}_ilp64_omp ex_${name}.f common_func.f)
  target_link_libraries(ex_f77_${name}_ilp64_omp PRIVATE nvpl::lapack_ilp64_omp)
  target_link_libraries(ex_f77_${name}_ilp64_omp PRIVATE OpenMP::OpenMP_Fortran)
  target_compile_options(
    ex_f77_${name}_ilp64_omp PRIVATE ${ILP64_FORTRAN_COMPILE_FLAG} ${FORTRAN_FREE_FORM_FLAG}
  )

  add_executable(ex_f77_${name}_lp64_omp ex_${name}.f common_func.f)
  target_link_libraries(ex_f77_${name}_lp64_omp PRIVATE nvpl::lapack_lp64_omp)
  target_link_libraries(ex_f77_${name}_lp64_omp PRIVATE OpenMP::OpenMP_Fortran)
  target_compile_options(
    ex_f77_${name}_lp64_omp PRIVATE ${FORTRAN_FREE_FORM_FLAG}
  )

  set(FORTRAN_EXES ${FORTRAN_EXES} ex_f77_${name}_ilp64_seq ex_f77_${name}_lp64_seq
                   ex_f77_${name}_ilp64_omp ex_f77_${name}_lp64_omp)
endmacro(addFortranExample)

# Fortran Examples
addFortranExample(getrf_nopivot)
addFortranExample(potrf)
addFortranExample(sgesv)
addFortranExample(dgesv)
addFortranExample(cgesv)
addFortranExample(zgesv)
addFortranExample(sgels)
addFortranExample(dgels)
addFortranExample(cgels)
addFortranExample(zgels)

# CTest
enable_testing()
foreach(exe IN LISTS FORTRAN_EXES)
  add_test(${exe} ${exe})
endforeach()

include(GNUInstallDirs)
install(TARGETS ${FORTRAN_EXES}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
