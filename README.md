# NVPL Samples

The NVIDIA Performance Libraries (NVPL) are a collection of high performance mathematical libraries optimized for the [NVIDIA Grace](https://www.nvidia.com/en-us/data-center/grace-cpu/) Arm v9 architecture.

These CPU-only libraries have no dependencies on CUDA or CTK, and are drop in replacements for standard C and Fortran mathematical APIs to achieve maximum performance on the Grace platform.


## Library Documentation and Examples

* [NVPL BLAS](nvpl_blas/README.md)
* [NVPL FFT](nvpl_fft/README.md)
* [NVPL LAPACK](nvpl_lapack/README.md)
* [NVPL RAND](nvpl_rand/README.md)
* [NVPL ScaLAPACK](nvpl_scalapack/README.md)
* [NVPL Sparse](nvpl_sparse/README.md)


## Support

### Systems

* Arch: aarch64-linux
* Platform: Arm SBSA
* Processors
    * Optimized
        * NVIDIA Grace (Arm-v9.0a Neoverse-v2)
        * AWS Graviton 3/3e (Arm-v8.4a Neoverse-v1)
    * Supported
        * AWS Graviton 2 (Arm-v8.2a Neoverse-n1)
        * Ampere Altra (Arm-v8.2a Neoverse-n1)
        * Any CPU with Arm-v8.2a+ microarch
* OS (Linux)
   * Ubuntu: 20.04, 22.04, 23.10
   * RHEL: (RHEL8) 8.8, (RHEL9) 9.2
   * Fedora: 35, 36, 37
   * SLES: (SLES15) 15.4, 15.5
   * AmazonLinux: 2, 2023
   * Generally any Linux OS with support for aarch64

### Compilers

* GCC
    * Supported: GCC-8 - GCC-13
* CLANG
    * Supported: CLANG-14 - CLANG-18, CLANG.grco
* NVHPC
    * Supported: nvhpc-23.9+

### Languages

* C (All libraries)
* C++ via C headers
* Fortran
  * gfortran ABI
  * Compatible with nvfortran-23.9 and later
  * `lp64` (32-bit integer) and `ilp64` (64-bit integer) ABI support

### OpenMP

* GNU OpenMP - libgomp.so
* CLANG - libomp.so
* NVHPC-23.09+ - libnvomp.so


## Installation


### CMake Usage

1. Use `find_package(nvpl REQUIRED COMPONENTS <lib...> )`
    * Available libs: blas, lapack, fft, rand

2. Use `target_link_libraries(my_tgt PUBLIC nvpl::<lib>_<opts>)` to link to NVPL target libraries
    * `<lib>` - the component library name, all lowercase.
    * `<opts>` - Options or variants.  See individual library documentation for details.
