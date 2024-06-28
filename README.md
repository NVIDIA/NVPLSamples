# NVPL Samples

The NVIDIA Performance Libraries (NVPL) are a collection of high performance mathematical libraries optimized for the [NVIDIA Grace](https://www.nvidia.com/en-us/data-center/grace-cpu/) Arm-v9.0a Neoverse-V2 architecture.

These CPU-only libraries have no dependencies on CUDA or CTK, and are drop in replacements for standard C and Fortran mathematical APIs allowing HPC applications to achieve maximum performance on the Grace platform.

The provided sample codes show how to call and link to NVPL Libraries in Fortran, C, and C++ applications and libraries.  Most examples use [CMake](#cmake-usage), but are easily modified for use in custom build environments.

* [NVPL Documentation](https://docs.nvidia.com/nvpl/)

## Installation

* [NVPL Downloads](https://developer.nvidia.com/nvpl-downloads/)
* Latest release: **NVPL-24.7**
## Library Samples

Samples are compatible with the latest nvpl release.  Compatibility with older releases is not guaranteed.

* [NVPL BLAS Samples](nvpl_blas/README.md)
* [NVPL FFT Samples](nvpl_fft/README.md)
* [NVPL LAPACK Samples](nvpl_lapack/README.md)
* [NVPL RAND Samples](nvpl_rand/README.md)
* [NVPL ScaLAPACK Samples](nvpl_scalapack/README.md)
* [NVPL Sparse Samples](nvpl_sparse/README.md)
* [NVPL Tensor Samples](nvpl_tensor/README.md)


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
   * Ubuntu: 20.04, 22.04, 24.04
   * RHEL: RHEL8, RHEL9
   * Fedora: 37, 38, 39, 40
   * SLES: SLES15
   * OpenSUSE/leap: 15.5
   * AmazonLinux: 2, 2023
   * Generally any Linux OS with support for aarch64

### Compilers

* GCC-8 - GCC-14+
* Clang-14 - Clang-18+
* [Clang for NVIDIA Grace](https://developer.nvidia.com/grace/clang/downloads): 16.x, 17.x, 18.x
* [NVIDA HPC Compilers](https://developer.nvidia.com/hpc-compilers>): 23.9 - 24.5

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

NVPL provides standard BLACS interfaces for the following MPI distributions.  See the [NVPL ScaLAPACK Samples Documentation](nvpl_scalapack/README.md) for details.

* [MPICH](https://www.mpich.org/): Runtime support for `>=mpich-3.4` and `<mpich-4.2`
* [OpenMPI-3.x](https://www.open-mpi.org/doc/v3.1/)
* [OpenMPI-4.x](https://www.open-mpi.org/doc/v4.1/)
* [OpenMPI-5.x](https://docs.open-mpi.org/en/v5.0.x/)
* [NVIDIA HPC-X](https://developer.nvidia.com/networking/hpc-x): Use `openmpi4` BLACS interface

## CMake Usage

NVPL provides [CMake Package
Config](https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html)
files for the each component library.

### Finding NVPL Packages

If NVPL was installed via the OS package manager under the `/usr`
directory, the NVPL packages will already be on the default
`CMAKE_PREFIX_PATH`. The `nvpl_ROOT` environment can be used to override
the default search path and force finding nvpl under a specific prefix.

The
[find_package()](https://cmake.org/cmake/help/latest/command/find_package.html)
command is used to find nvpl and any component libraries:

```cmake
find_package(nvpl)
```

Each NVPL component library found will print a brief status message with
important locations.

-   Variable `nvpl_FOUND` will be true if nvpl is successfully found
-   Variable `nvpl_VERSION` will contain the found version
-   Pass the `REQUIRED` keyword to raise an error if `nvpl` package is
    not found.
-   Regardless of the `COMPONENTS` keyword, all available nvpl component
    libraries installed in the same prefix will be found.
-   To raise an error if a particular component is not found, use
    `REQUIRED COMPONENTS ...`
-   Set `QUIET` to avoid printing status messages, or reporting an error
    if nvpl is not found
-   `find_package(nvpl)` can safely be called multiple times from
    different locations in a project.

### Linking to NVPL Packages

The NVPL component libraries provide [Imported Interface
Targets](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#imported-targets)
under the common `nvpl::` namespace. To add all the necessary flags to
compile and link against NVPL libraries, use the
[target_link_libraries()](https://cmake.org/cmake/help/latest/command/target_link_libraries.html)
command:

```cmake
target_link_libraries(my_target PUBLIC nvpl::<lib>_<opts>)
```

Here `<lib>` is the lowercase shorthand for the library and and `<opts>`
are defined by the library.

### NVPL Targets

NVPL component and target names use all-lowercase naming schema. See individual
libraries documentation for details on available options.

| Component | Targets | Options / Notes  |
| :--- | :--- | :--- |
| blas | `nvpl::blas_<int>_<thr>` | `<int>`: `lp64`, `ilp64`<br>`<thr>`: `seq`, `omp` |
| fft  | `nvpl::fftw` | FFTW API interface |
| lapack | `nvpl::lapack_<int>_<thr>` | `<int>`: `lp64`, `ilp64`<br>`<thr>`: `seq`, `omp` |
| rand | `nvpl::rand`<br>`nvpl::rand_mt` | Single-threaded<br>Multi-threaded (OpenMP) |
| scalapack | `nvpl::blacs_<int>_<mpi>`<br>`nvpl::scalapack_<int>` | `<int>`: `lp64`, `ilp64`<br>`<mpi>`: `mpich`, `openmpi3`,`openmpi4`, `openmpi5` |
| sparse | `nvpl::sparse` | |
| tensor | `nvpl::tensor` | |

### NVPL Variables

Each nvpl component library also exports variables

-   `nvpl_<comp>_VERSION` - Version of component library
-   `nvpl_<comp>_INCLUDE_DIR` - Full path to component headers directory
-   `nvpl_<comp>_LIBRARY_DIR` - Full path to component libraries
    directory


## LICENSE

These Sample codes are provided under the [NVIDIA Software license for NVPL SDK](./LICENSE).
