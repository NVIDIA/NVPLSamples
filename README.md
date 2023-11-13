# NVPL Samples

The NVIDIA Performance Libraries (NVPL) are a collection of high performance mathematical libraries optimized for the [NVIDIA Grace](https://www.nvidia.com/en-us/data-center/grace-cpu/) Arm-v9.0a Neoverse-V2 architecture.

These CPU-only libraries have no dependencies on CUDA or CTK, and are drop in replacements for standard C and Fortran mathematical APIs allowing HPC applications to achieve maximum performance on the Grace platform.

The provided sample codes show how to call and link to NVPL Libraries in Fortran, C, and C++ applications and libraries.  Most examples use [CMake](https://cmake.org/cmake/help/latest/), but are easily modified for use in custom build environments.

## Library Documentation and Examples

* [NVPL BLAS](nvpl_blas/README.md)
* [NVPL FFT](nvpl_fft/README.md)
* [NVPL LAPACK](nvpl_lapack/README.md)
* [NVPL RAND](nvpl_rand/README.md)
* [NVPL ScaLAPACK](nvpl_scalapack/README.md)
* [NVPL Sparse](nvpl_sparse/README.md)


## Support

### Systems

* Architecture: aarch64-linux
* Platform: Arm SBSA
* CPUs Supported
   * [NVIDIA Grace](https://www.nvidia.com/en-us/data-center/grace-cpu/) (Arm-v9.0a Neoverse-V2)
   * AWS Graviton 3/3e (Arm-v8.4a Neoverse-V1)
   * AWS Graviton 2 (Arm-v8.2a Neoverse-N1)
   * Ampere Altra (Arm-v8.2a Neoverse-N1)
   * Any CPU with Arm-v8.2a+ microarch
* OS (Linux)
   * Ubuntu: 20.04, 22.04, 23.10
   * RHEL: (RHEL8) 8.8, (RHEL9) 9.2
   * Fedora: 35, 36, 37
   * SLES: (SLES15) 15.4, 15.5
   * AmazonLinux: 2, 2023
   * Generally any Linux OS with support for aarch64

### Compilers

* GCC-8 - GCC-13+
* Clang-14 - Clang-18+
* [Clang for NVIDIA Grace](https://developer.nvidia.com/grace/clang/downloads)
* [NVIDA HPC Compilers](https://developer.nvidia.com/hpc-compilers>): 23.9+

### Languages

* C: All libraries
* C++: All libraries via C interfaces
* Fortran: GFortran ABI - Selected libraries
   * GFortran ABI
   * NVPL BLAS, LAPACK, and ScaLAPACK provide `lp64` and `ilp64` integer ABIs
   * See individual libraries samples documentation for further details

### OpenMP
All libraries support the following OpenMP runtime libraries. See individual libraries documentation for details and API extensions supporting nested parallelism.

* GCC: libgomp.so
* Clang: libomp.so
* NVHPC: libnvomp.so

### MPI

NVPL provides standard BLACS interfaces for the following MPI distributions.  See the NVPL ScaLAPACK documentation for details.

* `MPICH <https://www.mpich.org/>`_  : MPICH>=3.4 runtime supported
* `OpenMPI-3.x <https://www.open-mpi.org/doc/v3.1/>`_
* `OpenMPI-4.x <https://www-lb.open-mpi.org/doc/v4.1/>`_
* `OpenMPI-5.x <https://docs.open-mpi.org/en/v5.0.x/>`_
* `NVIDIA HPC-X <https://developer.nvidia.com/networking/hpc-x>`_ via OpenMPI-4 interface

## Installation

Download NVPL Libraries from [NVIDIA Developer](https://developer.nvidia.com/)

### CMake Usage

1. If NVPL has not been installed by standard package manager to `/usr`, ensure `nvpl_ROOT` environment variable is set to the prefix root of NVPL installation.  e.g., `nvpl_ROOT=/opt/nvpl`.  NVPL installations under `/usr` should be automatically found by CMake.

2. Use `find_package(nvpl REQUIRED COMPONENTS <lib...> )`
    * Available libs: `blas`, `fft`, `lapack`, `rand`, `scalpack`, `sparse`

3. Use `target_link_libraries(my_tgt PUBLIC nvpl::<lib>_<opts>)` to link to NVPL target libraries
    * `<lib>` - the component library name, all lowercase.
    * `<opts>` - Options or variants.
    * See individual library documentation for details specific to each library

## LICENSE

These Sample codes are provided under the [NVIDIA Software licenese for NVPL SDK](./LICENSE).
